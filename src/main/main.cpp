#include <iostream>
#include <sstream>

#include <cstring>

#include <main/version.hpp>
#include <core/worker.hpp>
#include <parsers/argparse.hpp>
#include <parsers/configparse.hpp>
#include <util/util.hpp>
#include <util/constants.hpp>


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
  if (arg == absctl::VERSION)
    display_version();
  else
    display_help();
}

int main(int argc, char* argv[]) {
  if (argc == 1 || !strcmp(argv[1], "help"))
    display_help();

  if (argc == 1 || !strcmp(argv[1], "version"))
    display_version();
  
  std::ifstream config_fd(absctl::CONF_PATH);
  std::vector<absctl::argument> args;
  std::stringstream config_contents;
  std::vector<absctl::token> tokens;
  absctl::configuration configuration;
  absctl::tokenizer tokenizer;
  absctl::parser parser;
  absctl::worker worker;

  config_contents << config_fd.rdbuf();
  tokenizer = absctl::tokenizer{config_contents.str()};
  tokens = tokenizer.tokenize();
  parser = absctl::parser{tokens};
  configuration = parser.parse();

  if (!absctl::parse_args(argc, argv, args))
    display_help();
  
  if (args.size() == 1)
    handle_single_argument(args[0].type);
  
  if (configuration.repo_directory.empty()) {
    exit(1);
  }

  worker.set_configuration(configuration);
  worker.add_packages(args);
  return worker.process(args[0].type);
}