#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "Location.hpp"
#include <cassert>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>

enum class TokenType {
  Eof,
  Identifier,
  Def,

  // Single Character Tokens
  LeftParen,
  RightParen,
  Star,
  Minus,
  Comma,
  Plus,
  Slash,
  Bang,
  Equal,
  Greater,
  Lesser,

  // Multi-character tokens
  BangEqual,
  EqualEqual,
  GreaterEqual,
  LesserEqual,

  // Data Types
  String,
  Number,

  // Reserved Keywords
  End,
  Then,
  And,
  Or,
  Let,
  Extern,
  If,
  Else,
  ElseIf,
  Return,
};

struct Token {

  std::optional<std::variant<double, std::string>> data;
  LineLoc lineLoc;
  TokenType type;

  // numbers and strings can only be defined on one line
  Token(TokenType type, double value, LineLoc lineLoc)
      : type(type), data(value), lineLoc(lineLoc) {}

  Token(TokenType type, std::string_view value, LineLoc lineLoc)
      : type(type), data(std::string(value)), lineLoc(lineLoc) {}

  Token(TokenType type, LineLoc lineLoc)
      : type(type), data(std::nullopt), lineLoc(lineLoc) {}

  double getNumber() {
    assert(data.has_value());
    assert(std::holds_alternative<double>(data.value()));

    return std::get<double>(data.value());
  }

  std::string_view getString() {
    assert(data.has_value());
    assert(std::holds_alternative<std::string>(data.value()));

    return std::get<std::string>(data.value());
  }
};

#endif // !TOKEN_HPP
