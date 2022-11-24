#include "Parser.hpp"
#include "AST.hpp"
#include "Lexer.hpp"
#include "Logger.hpp"
#include "Token.hpp"

#include <memory>
#include <optional>
#include <vector>

std::vector<NodeAST> Parser::parse() {
  std::vector<NodeAST> expressions = {};
  while (!isAtEnd()) {
    auto node = parseDeclaration();
    if (node)
      expressions.push_back(std::move(node.value()));
  };
  return expressions;
}

std::optional<NodeAST> Parser::parseDeclaration() {
  if (match(TokenType::Def))
    return parseFunctionDefinition();
  if (match(TokenType::Extern))
    return parsePrototype();

  return parseTopLevelExpr();
}

std::optional<FunctionAST> Parser::parseFunctionDefinition() {
  std::optional<PrototypeAST> proto = parsePrototype();
  if (!proto)
    return std::nullopt;
  if (auto expr = parseExpression()) {
    return FunctionAST(std::make_unique<PrototypeAST>(proto.value()), std::make_unique<ExprAST>(std::move(expr.value())));
  }
  return std::nullopt;
}

std::optional<FunctionAST> Parser::parseTopLevelExpr() {
  if (auto expr = parseExpression()) {
    auto proto = std::make_unique<PrototypeAST>("__anon_expr",  std::vector<std::string_view>());
    return FunctionAST(std::move(proto), std::make_unique<ExprAST>(std::move(expr.value())));
  };

  return std::nullopt;
}

std::optional<ExprAST> Parser::parseExpression() { 
  auto LHS = parsePrimary();
  if (!LHS)
    return std::nullopt;
  return parseBinOpRHS(0, std::move(LHS));
}

std::optional<ExprAST> Parser::parsePrimary() {

  if (match(TokenType::Identifier))
    return parseIdentifierExpr();
  if (match(TokenType::Number))
    return parseNumberExpr();
  if (match(TokenType::LeftParen))
    return parseGrouping();

  return std::nullopt;
}

std::optional<ExprAST> Parser::parseNumberExpr() {
  double value = previous().get<double>();
  return NumberExprAST(value);
}

std::optional<ExprAST> Parser::parseIdentifierExpr() {
  std::string_view idName = previous().get<std::string_view>();

  if (!match(TokenType::LeftParen)) // Simple variable ref.
    return VariableExprAST(idName);

  std::vector<ExprAST> args;
  if (!match(TokenType::RightParen)) {
    while (true) {

      if (auto arg = parseExpression())
        args.push_back(std::move(arg.value()));
      else
        return std::nullopt;

      if (match(TokenType::RightParen))
        break;

      if (!match(TokenType::Comma)) {
        LogError("Expected ',' in argument list");
        return std::nullopt;
      };
    }
  }

  return CallExprAST(idName, std::move(args));
}


std::optional<ExprAST> Parser::parseGrouping() {
  auto node = parseExpression();

  if (!node)
    return std::nullopt;

  if (!match(TokenType::RightParen))
    return std::nullopt;

  return node;
}

std::optional<ExprAST> Parser::parseBinOpRHS(int exprPrec, std::optional<ExprAST> LHS) {

  while (true) {
    int tokPrec = getTokenPrecedence();

    if (tokPrec < exprPrec)
      return LHS;

    TokenType binOp = peek().type;
    advance();
    auto RHS = parsePrimary();
    if (!RHS)
      return std::nullopt;

    int nextPrec = getTokenPrecedence();
    if (tokPrec < nextPrec) {
      RHS = parseBinOpRHS(tokPrec + 1, std::move(RHS));
      if (!RHS)
        return std::nullopt;
    }

    LHS = BinaryExprAST(std::make_unique<ExprAST>(std::move(LHS.value())), binOp, std::make_unique<ExprAST>(std::move(RHS.value())));
  }
}

std::optional<PrototypeAST> Parser::parsePrototype() {
  if (!match(TokenType::Identifier)) {
    LogError("Expected function name in prototype.");
    return std::nullopt;
  }

  std::string_view fnName = previous().get<std::string_view>();

  if (!match(TokenType::LeftParen)) {
    LogError("Expected '(' in prototype");
    return std::nullopt;
  }

  std::vector<std::string_view> args;

  if (!match(TokenType::RightParen)) {
    while (true) {
      if (match(TokenType::Identifier)) {
        std::string_view arg = previous().get<std::string_view>();
        args.push_back(std::move(arg));
      }

      if (match(TokenType::RightParen))
        break;

      if (!match(TokenType::Comma)) {
        LogError("Expected ',' in prototype");
        return std::nullopt;
      };
    };
  }

  return PrototypeAST(fnName, std::move(args));
}
