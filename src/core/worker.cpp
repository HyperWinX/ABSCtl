#include "parsers/package_db.hpp"
#include <iostream>
#include <filesystem>

#include <cstdio>

#include <alpm.h>

#include <core/checker.hpp>
#include <core/worker.hpp>
#include <core/logging/logging.hpp>
#include <parsers/argparse.hpp>
#include <unordered_map>
#include <util/constants.hpp>
#include <util/util.hpp>


void absctl::worker::set_configuration(absctl::configuration& conf) noexcept {
  config = conf;
}

void absctl::worker::set_logger(absctl::logger& log) noexcept {
  this->log = log;
}

std::fstream absctl::worker::open_config_file() noexcept {
  filename = get_tracked_files_path();
  if (!std::filesystem::exists(get_tracked_files_path())) {
    log.log(DEBUG, "No tracked packages list available, creating new");
    create_config();
  }
    
  return std::fstream{filename};
}

void absctl::worker::get_all_packages() noexcept {
  std::system(GET_ALL_PACKAGES_CMD);
  std::ifstream file(TMP_PACKAGES_PATH);
  std::string tmp;

  while (!file.eof()) {
    package tmp;
    std::getline(file, tmp.name, ' ');
    std::getline(file, tmp.version, '\n');
    packages.push_back(tmp);
  }
  file.close();
  std::filesystem::remove(TMP_PACKAGES_PATH);
}

void absctl::worker::parse_tracked_packages() noexcept {
  std::string str;

  while (!config_fd.eof()) {
    package tmp;
    std::getline(config_fd, tmp.name, ' ');
    if (tmp.name.length() < 2)
      continue;
    std::getline(config_fd, tmp.version, '\n');
    all_packages.push_back(tmp);
  }
}

void absctl::worker::save_config() noexcept {
  log.log(INFO, "Saving tracked packages...");
  config_fd.close();
  config_fd.open(filename, std::ios::in | std::ios::out | std::ios::trunc);

  for (package& pkg : all_packages)
    config_fd << pkg.name + ' ' + pkg.version + '\n';
  config_fd.flush();
  config_fd.close();
}

void absctl::worker::create_config() const noexcept {
  std::string path = get_config_dir_path();
  log.log(DEBUG, std::format("Creating config at {}", path).c_str());
  system(get_config_creation_cmd().c_str());
}

std::string absctl::worker::get_package_version(const std::string& name) noexcept {
  std::string version;
  if (system((GET_PACKAGE_VER_CMD_PART1 + name + GET_PACKAGE_VER_CMD_PART2).c_str()))
    exit(1);
  std::ifstream version_fd{TMP_PACKAGES_PATH};
  std::getline(version_fd, version, '\0');
  return version;
}

std::unordered_map<std::string, std::string> absctl::worker::get_all_versions() noexcept {
  std::unordered_map<std::string, std::string> map;
  system(PACMAN_GET_ALL);
  std::ifstream fd{TMP_PACKAGES_PATH};
  std::string name, version;
  
  while (!fd.eof()) {
    std::getline(fd, name, ' ');
    std::getline(fd, version, '\n');
    map[name] = version;
    name.clear();
    version.clear();
  }

  return map;
}

void absctl::worker::track_packages() noexcept {
  pkg_checker checker{packages.size(), log};
  std::vector<std::string> URLs{packages.size()};
  
  log.log(INFO, "Constructing URL strings...");
  for (size_t i = 0; i < packages.size(); ++i)
    URLs[i] = construct_url(packages[i].name);
  
  log.log(INFO, "Verifying all packages - making multiple requests....");
  checker.verify_packages(URLs, packages, all_packages_db);
  log.log(INFO, "Parsing all package versions...");
  auto map = get_all_versions();
  log.log(INFO, "Tracking all packages...");


  for (package& pkg : packages) {
    all_packages_db.add_pkg(pkg.name, map[pkg.name], db_type::TRACKED);
  }
  all_packages_db.commit_changes();
}

void absctl::worker::untrack_packages() noexcept {
  log.log(INFO, "Untracking packages...");
  for (package& pkg : packages) {
    all_packages_db.remove_pkg(pkg.name, db_type::TRACKED);
    all_packages_db.add_pkg(pkg.name, "", db_type::UNTRACKED);
  }
}

void absctl::worker::add_packages(std::vector<argument>& args) noexcept {
  for (size_t i = 1; i < args.size(); ++i){
    switch (args[i].type) {
      case arg_type::ALL:
        get_all_packages();
        return;
      case absctl::arg_type::VALUE:
        packages.push_back({args[i].value.value()});
        break;
      default:
        return;
    }
  } 
}

int absctl::worker::process(absctl::arg_type work_type) noexcept {
  switch(work_type) {
    case absctl::arg_type::TRACK:
      config_fd = open_config_file();

      if (!config_fd.is_open() || config_fd.bad())
        return 1;
      
      track_packages();
      save_config();
      break;
    case absctl::arg_type::UNTRACK:
      config_fd = open_config_file();

      if (!config_fd.is_open() || config_fd.bad())
        return 1;

      parse_tracked_packages();
      untrack_packages();
      save_config();
      break;
    case absctl::arg_type::BUILD:
      config_fd = open_config_file();

      if (!config_fd.is_open() || config_fd.bad())
        create_config();
      config_fd = open_config_file();
      if (!config_fd.is_open() || config_fd.bad())
        return 1;
      
      parse_tracked_packages();
      system(get_repos_mkdir_cmd(config.repo_directory).c_str());
      for (package& package : packages)
        system(get_build_command(package.name, config.repo_directory, config.makepkg_args).c_str());
      break;
    default:
      return 1;
  }
  all_packages_db.commit_changes();
  return 0;
}
