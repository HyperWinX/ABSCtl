#include <ostream>
namespace absctl {
  static constexpr char LOG_PREFIX[] = "[*] ";
  static constexpr char FATAL_PREFIX[] = "FATAL: ";
  static constexpr char DEBUG_PREFIX[] = "DEBUG: ";

  enum error_level {
    DEBUG = 5,
    INFO = 4,
    WARNING = 3,
    NONFATAL_ERROR = 2,
    FATAL = 1
  };

  class logger {
  private:
    error_level verbosity;

    template<typename char_t, typename traits>
    static constexpr std::basic_ostream<char_t, traits>& reset(std::basic_ostream<char_t, traits>& os) {
      return os << "\033[0m";
    }

    template<typename char_t, typename traits>
    static constexpr std::basic_ostream<char_t, traits>& fg_red(std::basic_ostream<char_t, traits>& os) {
      return os << "\033[31m";
    }

    template<typename char_t, typename traits>
    static constexpr std::basic_ostream<char_t, traits>& fg_yellow(std::basic_ostream<char_t, traits>& os) {
      return os << "\033[33m";
    }

    template<typename char_t, typename traits>
    static constexpr std::basic_ostream<char_t, traits>& fg_green(std::basic_ostream<char_t, traits>& os) {
      return os << "\033[32m";
    }

  public:
    void set_verbosity(error_level);
    void log(error_level, const char*);
  };
}