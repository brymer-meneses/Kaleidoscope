#ifndef PARSER_HPP
#define PARSER_HPP

#include "AST.hpp"
#include "Lexer.hpp"

#include <cctype>
#include <memory>
#include <string>
#include <vector>

class Parser {

private:
  Lexer mLexer;
  std::size_t mCurrent = 0;
  std::vector<Token> mTokens;
  std::string_view mSource;

public:
  Parser(std::string_view source) 
    : mLexer(source)
    , mTokens(mLexer.lex())
    , mSource(source) {}
  std::vector<NodeAST> parse();

private:
  std::optional<ExprAST> parseNumberExpr();
  std::optional<ExprAST> parseIdentifierExpr();
  std::optional<ExprAST> parsePrimary();
  std::optional<NodeAST> parseDeclaration();
  std::optional<ExprAST> parseExpression();
  std::optional<ExprAST> parseGrouping();
  std::optional<ExprAST> parseBinOpRHS(int exprPrec, std::optional<ExprAST> LHS);

  std::optional<PrototypeAST> parsePrototype();
  std::optional<FunctionAST> parseTopLevelExpr();
  std::optional<FunctionAST> parseFunctionDefinition();

  int getTokenPrecedence() {
    switch (this->peek().type) {
    case TokenType::Lesser:
    case TokenType::Greater:
    case TokenType::LesserEqual:
    case TokenType::GreaterEqual:
    case TokenType::EqualEqual:
      return 10;
    case TokenType::Plus:
    case TokenType::Minus:
      return 20;
    case TokenType::Star:
      return 40;
    default:
      return -1;
    }
  }

  inline Token &peek() { 
    return mTokens[mCurrent]; 
  }

  inline Token &previous() { 
    return mTokens[mCurrent - 1]; 
  }

  inline bool check(TokenType type) { 
    return peek().type == type; 
  }

  inline bool isAtEnd() { 
    return peek().type == TokenType::Eof;
  }

  void advance() {
    if (!isAtEnd())
      mCurrent += 1;
    return;
  }

  bool match(TokenType type) {
    if (check(type)) {
      mCurrent += 1;
      return true;
    };
    return false;
  }
};

#endif // !LEXER_HPP
