#include <cstring>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <checker.hpp>
#include <constants.hpp>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <iostream>

size_t absctl::_save_response(void* ptr, size_t size, size_t nmemb, std::string* f) {
  char buf[size * nmemb + 1];
  buf[size * nmemb] = '\0';
  f->operator=(buf);
  return nmemb * size;
}

void absctl::pkg_checker::verify_packages(std::vector<std::string>& URLs, std::vector<std::string>& packages) {
  int running = 0;

  for (size_t i = 0; i < packages.size(); ++i) {
    curl_easy_setopt(handles[i], CURLOPT_URL, URLs[i].c_str());
    curl_easy_setopt(handles[i], CURLOPT_WRITEFUNCTION, _save_response);
    curl_easy_setopt(handles[i], CURLOPT_WRITEDATA, &responses[i]);
    curl_multi_add_handle(curl, handles[i]);
  }

  curl_multi_perform(curl, &running);

  do {
    std::cout << "Asking..." << std::endl;
    curl_multi_poll(curl, nullptr, 0, 1000, nullptr);

    curl_multi_perform(curl, &running);
  } while (running);

  std::vector<std::string> failed_packages{};
  for (size_t i = 0; i < packages.size(); ++i) {
    if (responses[i] == FAIL_STR)
      failed_packages.push_back(packages[i]);
  }

  if (failed_packages.size() == 0)
    return;

  std::unordered_set<std::string> small_set(failed_packages.begin(), failed_packages.end());
  auto it = std::remove_if(packages.begin(), packages.end(), [&small_set](const std::string& str) {
    return small_set.find(str) != small_set.end();
  });
  packages.erase(it, packages.end());

}