#include "worker.hpp"
#include <cctype>
#include <cstdlib>
#include <vector>
#include <string>
#include <optional>
#include <configparse.hpp>
#include <iostream>

std::optional<char> absctl::tokenizer::peek() {
  if (i == contents.size()) return {};

  return contents[i];
}

char absctl::tokenizer::consume() {
  return contents[i++];
}

void absctl::tokenizer::skip() {
  ++i;
}

std::vector<absctl::token> absctl::tokenizer::tokenize() {
  std::vector<absctl::token> result;
  std::string buf;

  while (peek()) {
    std::cout << "loop\n";
    int a = 5;
    if (std::isalpha(peek().value())) {
      buf.push_back(consume());
      while (peek() && !std::isspace(peek().value()))
        buf.push_back(consume());

      result.push_back({token_assoc.at(buf.c_str())});
      buf.clear();
    } else if (peek().value() == '"') {
      skip();
      while (peek() && peek().value() != '"')
        buf.push_back(consume());
      skip();
      result.push_back({token_type::STRING, buf});
      buf.clear();
    } else if (std::isspace(peek().value()))
      skip();
    else {}
  }

  return result;
}

std::optional<absctl::token> absctl::parser::peek() {
  if (i == tokens.size()) return {};

  return tokens[i];
}

absctl::token absctl::parser::consume() {
  return tokens[i++];
}

void absctl::parser::skip() {
  ++i;
}

std::optional<absctl::statement> absctl::parser::parse_statement() {
  absctl::statement statement;
  if (!peek()) return {};
  if (!statement_assoc.contains(peek()->type)) {
    std::cout << "unknown statement type\n";
    exit(1);
  }
  statement.type = statement_assoc.at(consume().type);
  switch (statement.type) {
    case REPO_DIR:
    case EXIT_AFTER_ERROR:
    case MAKEPKG_ARGS:
      if (peek()->type != SET) {
        std::cout << "Failure\n";
        exit(1);
      }
      skip();
      break;
    default:
      std::cout << "Unknown statement type\n";
      exit(1);
  }
  if (peek()->type != STRING) {
    std::cout << "expected string\n";
    exit(1);
  }
  statement.value = consume().value;
  return statement;
}

absctl::configuration absctl::parser::parse() {
  configuration config;
  while (auto stmt = parse_statement()) {
    switch (stmt->type) {
      case REPO_DIR:
        config.repo_directory = stmt->value;
        break;
      case EXIT_AFTER_ERROR:
        config.exit_after_error = stmt->value == "true";
        break;
      case MAKEPKG_ARGS:
        config.makepkg_args = stmt->value;
        break;
      default:
        std::cout << "unknown statement type\n";
        exit(1);
    }
  }

  return config;
}