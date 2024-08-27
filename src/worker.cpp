#include "checker.hpp"
#include <cstdio>
#include <worker.hpp>
#include <argparse.hpp>
#include <iostream>
#include <filesystem>

std::fstream absctl::worker::open_config_file() noexcept {
  filename = "/home/" + get_username() + "/.config/absctl/tracked_files";
  return std::fstream{filename};
}

void absctl::worker::parse_config_file() noexcept {
  while (!config_fd.eof()) {
    std::string str;
    std::getline(config_fd, str, '\n');
    if (str.length() <= 1) continue;
    all_packages.push_back(str);
  }
}

void absctl::worker::save_config() noexcept {
  config_fd.close();
  std::filesystem::remove(filename);
  system(std::string{"touch " + filename}.c_str());
  config_fd = open_config_file();

  for (std::string& pkg : all_packages)
    config_fd << pkg + '\n';
  config_fd.flush();
  config_fd.close();
}

void absctl::worker::try_create_config() const noexcept {
  auto path = "/home/" + get_username() + "/.config/absctl";
  system(std::string("mkdir -p " + path + "&& touch " + path + "/tracked_files").c_str());
}

void absctl::worker::track_packages() noexcept {
  pkg_checker checker{};
  for (std::string& pkg : packages) {
    if (!checker.check_package("https://gitlab.archlinux.org/archlinux/packaging/packages/" + pkg + ".git"))
      std::cout << pkg << " failed" << std::endl;
    else {
      std::cout << "Trying to write\n";
      if (std::find(all_packages.begin(), all_packages.end(), pkg) == all_packages.end()) {
        std::cout << "Saving\n";
        all_packages.push_back(pkg);
      }
    }
  }
}

void absctl::worker::add_packages(std::vector<argument>& args) noexcept {
  for (size_t i = 1; i < args.size(); ++i)
    packages.push_back(args[i].value.value());
}

int absctl::worker::process(absctl::arg_type work_type) noexcept {
  switch(work_type) {
    case absctl::arg_type::TRACK:
      std::cout << "Tracking mode" << std::endl;
      config_fd = open_config_file();

      if (!config_fd.is_open() || config_fd.bad())
        try_create_config();
      config_fd = open_config_file();
      if (!config_fd.is_open() || config_fd.bad())
        return 1;
      
      parse_config_file();
      track_packages();
      save_config();
      break;
    case absctl::arg_type::UNTRACK:
      break;
    default:
      return 1;
  }
  return 0;
}