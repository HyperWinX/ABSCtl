#include <string>
#include <unordered_set>
#include <algorithm>

#include <cstring>

#include <curl/curl.h>
#include <curl/mprintf.h>

#include <core/checker.hpp>
#include <core/worker.hpp>
#include <util/constants.hpp>


size_t absctl::_save_response(void* ptr, size_t size, size_t nmemb, std::string* f) {
  *f = std::string{static_cast<char*>(ptr), size * nmemb};
  return nmemb * size;
}

void absctl::pkg_checker::verify_packages(std::vector<std::string>& URLs, std::vector<package>& packages) {
  std::unordered_set<std::string> failed_names;
  int running = 0;

  for (size_t i = 0; i < packages.size(); ++i) {
    handles[i] = curl_easy_init();
    curl_easy_setopt(handles[i], CURLOPT_URL, URLs[i].c_str());
    curl_easy_setopt(handles[i], CURLOPT_WRITEFUNCTION, _save_response);
    curl_easy_setopt(handles[i], CURLOPT_WRITEDATA, &responses[i]);
    curl_multi_add_handle(curl, handles[i]);
  }

  curl_multi_perform(curl, &running);

  do {
    curl_multi_poll(curl, nullptr, 0, 1000, nullptr);

    curl_multi_perform(curl, &running);
  } while (running);

  for (size_t i = 0; i < packages.size(); ++i) {
    if (responses[i] == FAIL_STR)
      failed_names.insert(packages[i].name);
  }

  if (failed_names.size() == 0)
    return;
  
  packages.erase(
    std::remove_if(packages.begin(), packages.end(), [&failed_names](const package& pkg) {
      return failed_names.count(pkg.name) > 0;
    }),
    packages.end()
  );
}