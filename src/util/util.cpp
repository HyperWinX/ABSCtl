#include <string>

#include <unistd.h>


std::string get_username() {
  char username[128] = {0};
  getlogin_r(username, 128);
  return {username};
}