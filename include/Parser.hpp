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
  Lexer lexer;
  std::size_t current = 0;
  std::vector<Token> tokens;
  std::string_view source;

public:
  Parser(std::string_view source) 
    : lexer(source)
    , tokens(lexer.lex())
    , source(source) {}
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
    return this->tokens[this->current]; 
  }

  inline Token &previous() { 
    return this->tokens[this->current - 1]; 
  }

  inline bool check(TokenType type) { 
    return this->peek().type == type; 
  }

  inline bool isAtEnd() { 
    return this->peek().type == TokenType::Eof;
  }

  void advance() {
    if (!isAtEnd())
      current += 1;
    return;
  }

  bool match(TokenType type) {
    if (check(type)) {
      this->current += 1;
      return true;
    };
    return false;
  }
};

#endif // !LEXER_HPP
