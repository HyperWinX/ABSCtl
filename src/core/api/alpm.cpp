#include <optional>
#include <string_view>

#include <core/api/alpm.hpp>

std::optional<std::string_view> absctl::alpm_api::get_pkg_version(std::string_view name) {
  const char* version = nullptr;

  alpm_list_t* dbs = syncdbs;
  while (dbs) {
    alpm_pkg_t* pkg = alpm_db_get_pkg((alpm_db_t*)dbs->data, name.begin());
    if (!pkg)
      dbs = dbs->next;
    else
      return version;
  }
  return {};
}