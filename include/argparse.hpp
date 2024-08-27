#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include <ext/eternal.hpp>

namespace absctl {
  enum arg_type : std::uint_fast8_t {
    TRACK,
    UNTRACK,
    VALUE,
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
    { "version", arg_type::VERSION },
    { "help", arg_type::HELP }
  });

  int parse_args(int argc, char* argv[], std::vector<argument>& args) noexcept;
}