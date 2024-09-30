#pragma once

#include <string>

#include <util/constants.hpp>


namespace absctl {
  std::string get_username() noexcept;

  inline constexpr std::string construct_url(std::string& pkg) noexcept {
    return URL_PART1 + pkg + URL_PART2;
  }

  inline std::string get_database_path() noexcept {
    return DATABASE_PATH_PART1 + get_username() + DATABASE_PATH_PART2;
  }

  inline std::string get_tracked_files_path() noexcept {
    return TRACKED_FILES_PATH_PART1 + get_username() + TRACKED_FILES_PATH_PART2;
  }

  inline std::string get_config_dir_path() noexcept {
    return CONFIG_DIR_PATH_PART1 + get_username() + CONFIG_DIR_PATH_PART2;
  }

  inline std::string get_repos_mkdir_cmd(std::string& path) noexcept {
    return MKDIR_CMD + path;
  }

  inline std::string get_config_creation_cmd() noexcept {
    std::string cmd = MKDIR_CMD;
    cmd += get_config_dir_path();
    cmd += CMD_JOIN;
    cmd += TOUCH_CMD;
    cmd += get_tracked_files_path();
    return cmd;
  }

  inline std::string get_build_command(std::string& pkg, std::string& repo_dir, std::string& makepkg_args) {
    std::string cmd = CD_CMD + repo_dir;
    cmd += CMD_JOIN;
    cmd += GIT_CLONE_CMD;
    cmd += URL_PART1;
    cmd += pkg;
    cmd += URL_PART2;
    cmd += CMD_JOIN;
    cmd += CD_CMD;
    cmd += pkg;
    cmd += CMD_JOIN;
    cmd += MAKEPKG_CMD + makepkg_args;
    return cmd;
  }
}

#include <parsers/configparse.hpp>
