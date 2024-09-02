#include <iostream>
#include <filesystem>
#include <unordered_set>

#include <cstdio>

#include <core/checker.hpp>
#include <core/worker.hpp>
#include <parsers/argparse.hpp>
#include <unordered_map>
#include <util/constants.hpp>
#include <util/util.hpp>


void absctl::worker::set_configuration(absctl::configuration& conf) noexcept {
  config = conf;
}

std::fstream absctl::worker::open_config_file() noexcept {
  filename = get_tracked_files_path();
  if (!std::filesystem::exists(get_tracked_files_path())) 
    create_config();
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
    std::getline(config_fd, tmp.version, '\n');
    all_packages.push_back(tmp);
  }
}

void absctl::worker::save_config() noexcept {
  std::cout << "[*] Saving tracked packages...\n";
  config_fd.close();
  config_fd.open(filename, std::ios::in | std::ios::out | std::ios::trunc);

  for (package& pkg : all_packages)
    config_fd << pkg.name + ' ' + pkg.version + '\n';
  config_fd.flush();
  config_fd.close();
}

void absctl::worker::create_config() const noexcept {
  std::string path = get_config_dir_path();
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
  pkg_checker checker{packages.size()};
  std::vector<std::string> URLs{packages.size()};
  
  std::cout << "[*] Constructing URL strings...\n";
  for (size_t i = 0; i < packages.size(); ++i)
    URLs[i] = construct_url(packages[i].name);
  
  std::cout << "[*] Verifying all packages - making multiple requests...\n";
  checker.verify_packages(URLs, packages);
  std::cout << "[*] Parsing all package versions...\n";
  auto map = get_all_versions();
  std::cout << "[*] Tracking all packages...\n";
  for (package& pkg : packages) {
    if (std::find_if(all_packages.begin(), all_packages.end(), [&pkg](const package& spkg) {
      return spkg.name == pkg.name;
    }) == all_packages.end()) {
      all_packages.push_back(pkg);
    }
  }
}

void absctl::worker::untrack_packages() noexcept {
  std::cout << "[*] Untracking packages...\n";
  all_packages.erase(
    std::remove_if(all_packages.begin(), all_packages.end(), [this](const package& pkg) {
      return std::find(this->packages.begin(), this->packages.end(), pkg) != this->packages.end();
    }),
    all_packages.end()
  );
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
      
      parse_tracked_packages();
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
  return 0;
}