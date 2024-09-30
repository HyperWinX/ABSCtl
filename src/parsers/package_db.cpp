#include <string>

#include <parsers/package_db.hpp>

int absctl::database_connector::execute_query(std::string query) {
  return database.exec(query);
}

void absctl::database_connector::commit_changes() {
  transaction.commit();
}