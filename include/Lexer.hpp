#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"

#include <string>
#include <vector>

class Lexer {

private:
  const std::string_view mSource;
  std::size_t mCurrent = 0;
  std::size_t mStart = 0;
  std::size_t mLine = 1;
  bool mHadError = false;

public:
  Lexer(std::string_view source) : mSource(source){};

  std::vector<Token> lex();

private:
  char advance() {
    if (mCurrent >= mSource.length())
      return EOF;

    mCurrent += 1;
    return this->mSource[mCurrent - 1];
  }

  char peek() {
    if (mCurrent >= mSource.length())
      return EOF;

    return mSource[mCurrent];
  }

  bool match(char c) {
    if (peek() == c) {
      mCurrent += 1;
      return true;
    }
    return false;
  }

  char peekNext() {
    if (mCurrent + 1 >= mSource.length())
      return EOF;

    return mSource[mCurrent + 1];
  }

  bool isAtEnd() {
    if (mCurrent >= mSource.length())
      return true;

    return false;
  }

  std::optional<Token> lexSingleToken();
  LineLoc getLineLoc() { return LineLoc(mStart, mCurrent - 1, mLine); };
  Token lexString();
  Token lexIdentifier();
  Token lexNumber();
};

#endif // !LEXER_HPP
