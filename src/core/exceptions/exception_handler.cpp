#include <iostream>

#include <cstdlib>

#include <core/exceptions/exception_handler.hpp>

[[noreturn]] void absctl::exception_handler::_throw(absctl::exception_type type, const char* msg) {
  const char* final_msg = msg == nullptr ? exception_messages.at(type).c_str() : msg;

  log.log(error_level::FATAL, final_msg);
  exit(1);
}
