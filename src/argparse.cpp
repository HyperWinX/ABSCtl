#include <argparse.hpp>

#include <cstring>

int absctl::parse_args(int argc, char* argv[], std::vector<argument>& args) noexcept {
  if (argc == 1) return argc;

  args.push_back({arg_assoc.at(argv[1])});

  if (argc == 3 && !strcmp(argv[2], "*")) {
    args.push_back({arg_type::ALL});
    return 0;
  }
  for (size_t i = 2; i < argc; ++i)
    args.push_back({arg_type::VALUE, argv[i]});

  return 0;
}