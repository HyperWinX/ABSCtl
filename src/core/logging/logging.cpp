#include <iostream>

#include <core/logging/logging.hpp>
#include <stdexcept>

void absctl::logger::set_verbosity(error_level err_lvl) {
  verbosity = err_lvl;
}

void absctl::logger::log(absctl::error_level err_lvl, const char* msg) {
  switch (err_lvl) {
    case FATAL:
      std::cout << fg_red << FATAL_PREFIX << msg << reset << '\n';
      break;
    case NONFATAL_ERROR:
      std::cout << fg_red << LOG_PREFIX << msg << reset << '\n';
      break;
    case WARNING:
      if (verbosity < WARNING) return;
      std::cout << fg_yellow << LOG_PREFIX << msg << reset << '\n';
      break;
    case INFO:
      if (verbosity < INFO) return;
      std::cout << LOG_PREFIX << msg << reset << '\n';
      break;
    case DEBUG:
      if (verbosity < DEBUG) return;
      std::cout << DEBUG_PREFIX << msg << reset << '\n';
      break;
    default:
      throw std::runtime_error("Invalid error level");
  }
}