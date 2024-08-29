#pragma once

#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/multi.h>
#include <vector>
#include <string>
#include <cstdint>

namespace absctl {
  size_t _save_response(void* ptr, size_t size, size_t nmemb, std::string* f);
  class pkg_checker {
  private:
    struct transfer {
      CURL* easy;
      std::uint32_t num;
      FILE* out;
    };

    CURLM* curl;
    CURLcode res;
    transfer* transfers;
    std::vector<CURL*> handles;
    std::vector<std::string> responses;
    size_t transfers_cnt;

    void setup_transfer(transfer*, int);
  public:
    pkg_checker(size_t cnt) : transfers_cnt(cnt) {
      curl = curl_multi_init();
      handles = std::vector<CURL*>{cnt};
      responses = std::vector<std::string>{cnt};

      curl_global_init(CURL_GLOBAL_DEFAULT);

    }

    void verify_packages(std::vector<std::string>& URLs, std::vector<std::string>& packages);

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