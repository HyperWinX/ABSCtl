#pragma once
#include <alpm.h>
#include <alpm_list.h>
#include <constants.hpp>
#include <mapbox/eternal.hpp>
#include <core/logging/logging.hpp>
#include <util/constants.hpp>

#include <hsqlite.hpp>


namespace absctl {
  namespace sql_queries {
    static constexpr char TRACKED_PACKAGES_TBL_CREATE[] = "CREATE TABLE IF NOT EXISTS tracked_packages (id INTEGER PRIMARY KEY AUTOINCREMENT, pkg_name VARCHAR(32), pkg_version VARCHAR(32));";
    static constexpr char UNTRACKED_PACKAGES_TBL_CREATE[] = "CREATE TABLE IF NOT EXISTS untracked_packages (id INTEGER PRIMARY KEY AUTOINCREMENT, pkg_name VARCHAR(32), pkg_version VARCHAR(32));";
  }

  enum db_type {
    TRACKED,
    UNTRACKED
  };

  constexpr const auto db_types = mapbox::eternal::map<db_type, mapbox::eternal::string>({
    { db_type::TRACKED, "tracked_packages" },
    { db_type::UNTRACKED, "untracked_packages" }
  });

  class database_connector {
  private:
    hsqlite::database db;
    hsqlite::transaction tr;
    absctl::logger& log;
    alpm_handle_t* handle;
    alpm_list_t* syncdbs;
    alpm_db_t* localdb;
  public:
    database_connector(std::string_view db_file, absctl::logger& log) : db(db_file, hsqlite::flag::OPEN_READWRITE | hsqlite::flag::OPEN_CREATE | hsqlite::flag::OPEN_EXRESCODE), tr(db), log(log) {
      db.exec(sql_queries::TRACKED_PACKAGES_TBL_CREATE);
      db.exec(sql_queries::UNTRACKED_PACKAGES_TBL_CREATE);
      commit_changes();

      handle = alpm_initialize(ALPM_ROOT, ALPM_DB_PATH, nullptr);
      syncdbs = alpm_get_syncdbs(handle);
      localdb = alpm_get_localdb(handle);
    }

    bool package_exists(std::string_view name, db_type type);
    void remove_pkg(std::string_view name, db_type type);
    void add_pkg(std::string_view name, std::string_view version, db_type type);
    int execute_query(std::string query);
    void commit_changes();

    ~database_connector() = default;
  };
}