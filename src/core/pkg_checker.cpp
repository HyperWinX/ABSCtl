#include <string>
#include <unordered_set>
#include <algorithm>
#include <iostream>

#include <cstring>

#include <curl/curl.h>
#include <curl/mprintf.h>

#include <core/checker.hpp>
#include <util/constants.hpp>


size_t absctl::_save_response(void* ptr, size_t size, size_t nmemb, FILE* f) {
  char buf[size * nmemb + 1];
  buf[size * nmemb] = '\0';
  std::cout << "Data: " << buf << std::endl;
  *reinterpret_cast<std::string*>(f) = buf;
  std::cout << "Buffer: " << reinterpret_cast<std::string*>(f)->c_str() << std::endl;
  return nmemb * size;
}

void absctl::pkg_checker::verify_packages(std::vector<std::string>& URLs, std::vector<std::string>& packages) {
  std::unordered_set<std::string> small_set;
  std::vector<std::string> failed_packages;
  int running = 0;

  for (size_t i = 0; i < packages.size(); ++i) {
    handles[i] = curl_easy_init();
    curl_easy_setopt(handles[i], CURLOPT_URL, URLs[i].c_str());
    std::cout << "URL: " << URLs[i] << std::endl;
    curl_easy_setopt(handles[i], CURLOPT_WRITEFUNCTION, _save_response);
    curl_easy_setopt(handles[i], CURLOPT_WRITEDATA, reinterpret_cast<FILE*>(&responses[i]));
    curl_easy_setopt(handles[i], CURLOPT_VERBOSE, 1L);
    curl_multi_add_handle(curl, handles[i]);
  }

  curl_multi_perform(curl, &running);

  do {
    std::cout << "Polling...\n";
    CURLMcode code = curl_multi_poll(curl, nullptr, 0, 1000, nullptr);
    if (code != CURLM_OK)
      std::cout << "Some issues happened\n";

    curl_multi_perform(curl, &running);
  } while (running);

  for (size_t i = 0; i < packages.size(); ++i) {
    std::cout << "Response: " << responses[i] << std::endl;
    std::cout << "FAIL_STR: " << FAIL_STR << std::endl;
    if (responses[i] == FAIL_STR)
      failed_packages.push_back(packages[i]);
  }

  if (failed_packages.size() == 0)
    return;

  small_set = std::unordered_set<std::string>(failed_packages.begin(), failed_packages.end());
  
  auto it = std::remove_if(packages.begin(), packages.end(), [&small_set](const std::string& str) {
    return small_set.find(str) != small_set.end();
  });
  packages.erase(it, packages.end());
}