#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"

#include <string>
#include <vector>

class Lexer {

private:
  const std::string_view source;
  std::size_t current = 0;
  std::size_t start = 0;
  std::size_t line = 1;
  bool hadError;

public:
  Lexer(std::string_view source) : source(source){};

  std::vector<Token> lex();

private:
  char advance() {
    if (this->current >= source.length())
      return EOF;

    this->current += 1;
    return this->source[this->current - 1];
  }

  char peek() {
    if (this->current >= source.length())
      return EOF;

    return this->source[this->current];
  }

  bool match(char c) {
    if (peek() == c) {
      this->current += 1;
      return true;
    }
    return false;
  }

  char peekNext() {
    if (this->current + 1 >= source.length())
      return EOF;

    return this->source[this->current + 1];
  }

  bool isAtEnd() {
    if (this->current >= source.length())
      return true;

    return false;
  }

  std::optional<Token> lexSingleToken();
  LineLoc getLineLoc() { return LineLoc(start, current - 1, line); };
  Token lexString();
  Token lexIdentifier();
  Token lexNumber();
};

#endif // !LEXER_HPP
