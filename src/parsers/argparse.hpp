#pragma once

#include <optional>
#include <string>
#include <vector>

#include <cstdint>

#include <core/logging/logging.hpp>
#include <mapbox/eternal.hpp>


namespace absctl {
  enum arg_type : std::uint_fast8_t {
    TRACK,
    UNTRACK,
    BUILD,
    VALUE,
    VERBOSITY,
    ALL,
    VERSION,
    HELP
  };

  struct argument {
    arg_type type;
    std::optional<std::string> value;
  };

  MAPBOX_ETERNAL_CONSTEXPR const auto arg_assoc = mapbox::eternal::map<mapbox::eternal::string, arg_type>({
    { "track", arg_type::TRACK },
    { "untrack", arg_type::UNTRACK },
    { "build", arg_type::BUILD },
    { "version", arg_type::VERSION },
    { "help", arg_type::HELP }
  });

  MAPBOX_ETERNAL_CONSTEXPR const auto verbosity_args = mapbox::eternal::map<mapbox::eternal::string, error_level>({
    { "--verror", error_level::NONFATAL_ERROR },
    { "--vwarning", error_level::WARNING },
    { "--vinfo", error_level::INFO },
    { "--vdebug", error_level::DEBUG }
  });

  int parse_args(int argc, char* argv[], std::vector<argument>& args) noexcept;
}
