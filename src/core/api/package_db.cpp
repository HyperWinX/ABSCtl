#include "constants.hpp"
#include "core/logging/logging.hpp"
#include <string>
#include <format>

#include <core/api/package_db.hpp>

int absctl::database_connector::execute_query(std::string query) {
  return db.exec(query);
}

void absctl::database_connector::commit_changes() {
  hsqlite::error_code code = tr.commit();
  auto t = sqlite3_errmsg(db.db);
}

bool absctl::database_connector::package_exists(std::string_view name, db_type type) {
  auto query = std::format("SELECT COUNT(*) FROM {} WHERE pkg_name = \"{}\";", db_types.at(type).c_str(), name);
  hsqlite::statement stmt(db, query.c_str(), query.length());
  stmt.step();
  auto t = stmt.get_column_int<0>();
  return static_cast<bool>(t);
}

void absctl::database_connector::remove_pkg(std::string_view name, db_type type) {
  db.exec(std::format("DELETE FROM {} WHERE pkg_name = \"{}\";", db_types.at(type).c_str(), name));
}

void absctl::database_connector::add_pkg(std::string_view name, std::string_view version, db_type type) {
  if (package_exists(name, type)) {
    log.log(DEBUG, std::format("Package {} already exists in the database {}", name, db_types.at(type).c_str()).c_str());
    return;
  }

  db.exec(std::format("INSERT INTO {}(pkg_name, pkg_version) VALUES (\"{}\", \"{}\");", db_types.at(type).c_str(), name, version));
}
