#pragma once

namespace absctl {
  static constexpr char FAIL_STR[] = "<html><body>You are being <a href=\"https://gitlab.archlinux.org/users/sign_in\">redirected</a>.</body></html>";
  static constexpr char GET_ALL_PACKAGES_CMD[] = "pacman -Q | awk '{print $1}' > /tmp/all_packages";
  static constexpr char TMP_PACKAGES_PATH[] = "/tmp/all_packages";
  static constexpr char CONF_PATH[] = "/etc/absctl.conf";
  static constexpr char GIT_CLONE_CMD[] = "git clone ";
  static constexpr char DEFAULT_MAKEPKG_ARGS[] = "-si";
  static constexpr char MAKEPKG_CMD[] = "makepkg ";
  static constexpr char MKDIR_CMD[] = "mkdir -p ";
  static constexpr char TOUCH_CMD[] = "touch ";
  static constexpr char CMD_JOIN[] = " && ";
  static constexpr char CD_CMD[] = "cd ";

  static constexpr char URL_PART1[] = "https://gitlab.archlinux.org/archlinux/packaging/packages/";
  static constexpr char URL_PART2[] = ".git";

  static constexpr char TRACKED_FILES_PATH_PART1[] = "/home/";
  static constexpr char TRACKED_FILES_PATH_PART2[] = "/.config/absctl/tracked_files";

  static constexpr char CONFIG_DIR_PATH_PART1[] = "/home/";
  static constexpr char CONFIG_DIR_PATH_PART2[] = "/.config/absctl";
}