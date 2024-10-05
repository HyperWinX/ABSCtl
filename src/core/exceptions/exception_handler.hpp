#pragma once

#include <core/logging/logging.hpp>
#include <mapbox/eternal.hpp>


namespace absctl {
  enum exception_type {
    UNKNOWN_ACTION,
    CONFIG_OPEN_FAIL,
    ALPM_INIT_FAIL,
    ALPM_SYNCDB_INIT_FAIL,
    ALPM_LOCALDB_INIT_FAIL
  };

  constexpr static auto exception_messages = mapbox::eternal::map<exception_type, mapbox::eternal::string>({
    { exception_type::UNKNOWN_ACTION, "Unknown action selected!" },
    { exception_type::CONFIG_OPEN_FAIL, "Failed to open config file!" },
    { exception_type::ALPM_INIT_FAIL, "Failed to initialize ALPM!" },
    { exception_type::ALPM_SYNCDB_INIT_FAIL, "Failed to initialize syncdbs list!" },
    { exception_type::ALPM_LOCALDB_INIT_FAIL, "Failed to initialize localdb!" }
  });

  class exception_handler{
  private:
    logger& log;
  public:
    exception_handler(logger& log) : log(log){}
    ~exception_handler() = default;

    [[noreturn]] void _throw(exception_type type, const char* msg = NULL);
  };
}
