#include <string>

#include <unistd.h>

#include <util/constants.hpp>
#include <util/util.hpp>


std::string absctl::get_username() noexcept {
  char username[128] = {0};
  getlogin_r(username, 128);
  return {username};
}
