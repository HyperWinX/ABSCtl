#pragma once

#include "SQLiteCpp/Database.h"
#include <SQLiteCpp/SQLiteCpp.h>

namespace absctl {
  namespace sql_queries {
    static constexpr char TRACKED_PACKAGES_TBL_CREATE[] = "CREATE TABLE IF NOT EXISTS tracked_packages (id INTEGER PRIMARY KEY AUTO_INCREMENT, pkg_name VARCHAR(32), pkg_version VARCHAR(32))";
  }

  class database_connector {
  private:
    SQLite::Database database;
    SQLite::Transaction transaction;
  public:
    database_connector(std::string db_file) : database(db_file, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), transaction(database) {
      database.exec(sql_queries::TRACKED_PACKAGES_TBL_CREATE);
      commit_changes();
    }

    int execute_query(std::string query);
    void commit_changes();

    ~database_connector() = default;
  };
}