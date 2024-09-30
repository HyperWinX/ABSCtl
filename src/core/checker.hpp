#pragma once

#include "parsers/package_db.hpp"
#include <vector>
#include <string>

#include <curl/curl.h>
#include <curl/multi.h>

#include <core/worker.hpp>
#include <core/logging/logging.hpp>


namespace absctl {
  size_t _save_response(void* ptr, size_t size, size_t nmemb, std::string* f);
  class pkg_checker {
  private:

    CURLM* curl;
    CURLcode res;
    std::vector<CURL*> handles;
    std::vector<std::string> responses;
    size_t transfers_cnt;
    logger& log;
    
  public:
    pkg_checker(size_t cnt, logger& log) : transfers_cnt(cnt), handles(cnt), responses(cnt), log(log) {
      curl = curl_multi_init();
      curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    void verify_packages(std::vector<std::string>& URLs, std::vector<package>& packages, database_connector& connector);

    ~pkg_checker() {
      for (size_t i = 0; i < transfers_cnt; ++i) {
        curl_multi_remove_handle(curl, handles[i]);
        curl_easy_cleanup(handles[i]);
      }

      curl_multi_cleanup(curl);
      curl_global_cleanup();
    }
  };
}