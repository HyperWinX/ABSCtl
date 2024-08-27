#include <cstring>
#include <curl/curl.h>
#include <checker.hpp>
#include <constants.hpp>
#include <string>
#include <iostream>

size_t absctl::_write_to_stream(void* ptr, size_t size, size_t nmemb, FILE* f) {
  *reinterpret_cast<std::strstream*>(f) << std::string(static_cast<char*>(ptr));
  return nmemb * size;
}

bool absctl::pkg_checker::check_package(std::string url) {
  char buffer[256] = {0};
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, buffer);
  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    std::cout << "request failed\n";
    std::cout << buffer;
    return false;
  }
  std::cout << data.str() << std::endl;
  return strcmp(data.str(), FAIL_STR);
}