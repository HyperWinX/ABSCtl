#include <cstring>

#include <parsers/argparse.hpp>


int absctl::parse_args(int argc, char* argv[], std::vector<argument>& args) noexcept {
  if (argc == 1) return 0;

  args.push_back({.type=arg_assoc.at(argv[1]), .value=""});

  if (argc == 3 && !strcmp(argv[2], "*")) {
    args.push_back({arg_type::ALL});
    return 1;
  }
  for (size_t i = 2; i < argc; ++i)
    args.push_back({verbosity_args.contains(argv[i]) ? arg_type::VERBOSITY : arg_type::VALUE, argv[i]});

  return 1;
}
