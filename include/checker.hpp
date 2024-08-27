#pragma once

#include <curl/curl.h>
#include <curl/easy.h>
#include <strstream>

namespace absctl {
  size_t _write_to_stream(void* ptr, size_t size, size_t nmemb, FILE* f);
  class pkg_checker {
  private:
    CURL* curl;
    CURLcode res;
    std::strstream data;
  public:
    pkg_checker() {
      curl = curl_easy_init();
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write_to_stream);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<FILE*>(&data));
    }

    bool check_package(std::string url);

    ~pkg_checker() {
      curl_easy_cleanup(curl);
    }
  };
}