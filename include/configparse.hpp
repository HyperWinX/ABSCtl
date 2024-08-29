#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

#include <worker.hpp>
#include <ext/eternal.hpp>

namespace absctl {
  enum token_type {
    REPO_DIRECTORY,
    ERROR_IS_FATAL,
    MAKEPKG_ARGUMENTS,
    
    ADD,
    SET,

    STRING
  };

  enum statement_type {
    REPO_DIR,
    EXIT_AFTER_ERROR,
    MAKEPKG_ARGS
  };

  struct statement {
    statement_type type;
    std::string value;
  };
  
  struct token {
    token_type type;
    std::string value;
  };

  constexpr const auto token_assoc = mapbox::eternal::map<mapbox::eternal::string, token_type>({
    {"repo_dir", token_type::REPO_DIRECTORY},
    {"exit_after_error", token_type::ERROR_IS_FATAL},
    {"makepkg_args", token_type::MAKEPKG_ARGUMENTS},
    {"add", token_type::ADD},
    {"set", token_type::SET}
  });
  
  inline const static std::unordered_map<token_type, statement_type> statement_assoc {
    std::make_pair(token_type::REPO_DIRECTORY, statement_type::REPO_DIR),
    std::make_pair(token_type::ERROR_IS_FATAL, statement_type::EXIT_AFTER_ERROR),
    std::make_pair(token_type::MAKEPKG_ARGUMENTS, statement_type::MAKEPKG_ARGS)
  };

  class tokenizer {
  private:
    size_t i;
    std::string contents;

    [[nodiscard]] std::optional<char> peek();
    [[nodiscard]] char consume();
    void skip();
  
  public:
    tokenizer(std::string contents) : i(0), contents(contents){}
    std::vector<token> tokenize();
  };

  class parser {
  private:
    size_t i;
    std::vector<token> tokens;

    [[nodiscard]] std::optional<token> peek();
    [[nodiscard]] token consume();
    void skip();
    std::optional<statement> parse_statement();

  public:
    parser(std::vector<token>& tokens) : i(0), tokens(tokens){}
    configuration parse();
  };
}