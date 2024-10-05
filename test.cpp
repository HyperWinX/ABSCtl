#include <alpm.h>
#include <alpm_list.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cstdint>

int main() {
  std::ifstream file("/etc/pacman.conf");
  std::vector<std::string> repos;
  std::string line;
  std::uint32_t idx = 1;
  while (!file.eof()) {
    std::getline(file, line);
    if (line.length() <= 2) continue;
    if (line[0] != '[') continue;
    else {
      std::string repo;
      while (idx < line.length() && line.at(idx) != ']') {
        repo.push_back(line[idx++]);
      }
      idx = 1;
      if (repo == "options") continue;
      if (repo.length() < 2) continue;

      repos.push_back(repo);
    }
  }

  std::vector<alpm_db_t*> db_handles;
  alpm_errno_t err = ALPM_ERR_OK;
  alpm_handle_t* handle = alpm_initialize("/", "/var/lib/pacman", &err);
  for (auto repo : repos) {
    alpm_db_t* repo_handle = alpm_register_syncdb(handle, repo.c_str(), ALPM_DB_USAGE_ALL);
    alpm_errno_t err = alpm_errno(handle);
    assert(repo_handle);
    db_handles.push_back(repo_handle);
  }

  alpm_list_t* dbs = alpm_get_syncdbs(handle);
  while (dbs) {
    alpm_pkg_t* pkg = alpm_db_get_pkg((alpm_db_t*)dbs->data, "linux-zen");
    if (!pkg)
      dbs = dbs->next;
    else {
      std::cout << "Package: linux " << "found in repo: " << alpm_db_get_name((alpm_db_t*)dbs->data) << ", version: " << alpm_pkg_get_version(pkg) << '\n';
      break;
    }
  }

  alpm_db_t* localdb = alpm_get_localdb(handle);
  alpm_pkg_t* pkg = alpm_db_get_pkg(localdb, "linux-zen");
  std::cout << "Package: linux found in repo: " << alpm_db_get_name(localdb) << ", version: " << alpm_pkg_get_version(pkg) << '\n';
}