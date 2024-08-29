#include <iostream>
#include <cstring>
#include <version.hpp>
#include <util.hpp>
#include <argparse.hpp>
#include <worker.hpp>
#include <configparse.hpp>
#include <sstream>

[[noreturn]] void display_help() {
  std::cout << "Usage: absctl [COMMAND] [OPTIONS]\n\n";
  std::cout << "Simple Arch Linux Build System controller for building packages from sources with makepkg.\n\n";
  std::cout << "COMMANDS\n"
            << "\ttrack     Start tracking package\n"
            << "\tuntrack   Untrack package\n"
            << "\tbuild     Clone source code and build package\n"
            << "\tversion   Show program version\n"
            << "\thelp      Show this information"
            << std::endl;
  exit(0);
}

[[noreturn]] void display_version() {
  std::cout << "ABSCtl version " << VERSION << std::endl;
  exit(0);
}

[[noreturn]] void handle_single_argument(absctl::arg_type arg) {
  switch(arg) {
    case absctl::VERSION:
      display_version();
    default:
      display_help();
  }
}

int main(int argc, char* argv[]) {
  if (argc == 1 || !strcmp(argv[1], "help"))
    display_help();

  if (argc == 1 || !strcmp(argv[1], "version"))
    display_version();
  
  std::ifstream config("/etc/absctl.conf");
  std::stringstream conf;
  conf << config.rdbuf();
  absctl::tokenizer tokenizer{conf.str()};
  auto tokens = tokenizer.tokenize();
  absctl::parser parser{tokens};
  absctl::configuration config_info = parser.parse();
  std::cout << "parsed config\n";
  std::vector<absctl::argument> args{};

  if (absctl::parse_args(argc, argv, args))
    display_help();
  
  if (args.size() == 1)
    handle_single_argument(args[0].type);

  absctl::worker worker{config_info};
  worker.add_packages(args);
  return worker.process(args[0].type);
}