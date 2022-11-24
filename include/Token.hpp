#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "Location.hpp"
#include <cassert>
#include <optional>
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

  std::optional<std::variant<double, std::string_view>> data;
  FileLoc fileLoc;
  TokenType type;

  // numbers and strings can only be defined on one line
  Token(TokenType type, double value, LineLoc lineLoc)
      : type(type), data(value), fileLoc{lineLoc} {}

  Token(TokenType type, std::string_view value, LineLoc lineLoc)
      : type(type), data(value), fileLoc{lineLoc} {}

  Token(TokenType type, LineLoc lineLoc)
      : type(type), data(std::nullopt), fileLoc{lineLoc} {}

  template <typename T> 
  T get() {
    assert(data.has_value() && "data needs to have value" );
    assert(std::holds_alternative<T>(data.value()) && "must be same type");
    return std::get<T>(data.value());
  }
};

#endif // !TOKEN_HPP
