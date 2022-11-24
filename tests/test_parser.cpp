
#include "AST.hpp"
#include "Parser.hpp"

#include "gtest/gtest.h"
#include <memory>

TEST(Parser, NumberExpr) {

  Parser parser = Parser("42");
  auto statements = parser.parse();

  const auto expected =
      FunctionAST(std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string_view>()),
                  std::make_unique<ExprAST>(NumberExprAST(42)));

  ASSERT_EQ(statements.size(), 1);
  ASSERT_EQ(statements[0], expected);
}

TEST(Parser, VariableExpr) {

  Parser parser = Parser("varname123");

  auto statements = parser.parse();

  const auto expected =
      FunctionAST(std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string_view>()),
                  std::make_unique<ExprAST>(VariableExprAST("varname123")));

  ASSERT_EQ(statements.size(), 1);
  ASSERT_EQ(statements[0], expected);
}

TEST(Parser, BinaryExpr) {

  Parser parser = Parser("55 + 45");

  auto statements = parser.parse();

  const auto expected =
      FunctionAST(std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string_view>()),
                  std::make_unique<ExprAST>(
                      BinaryExprAST(std::make_unique<ExprAST>(NumberExprAST(55)),
                                    TokenType::Plus, 
                                    std::make_unique<ExprAST>(NumberExprAST(45)))));

  ASSERT_EQ(statements.size(), 1);
  ASSERT_EQ(statements[0], expected);
}

TEST(Parser, GroupingBinaryExpr) {

  Parser parser = Parser("55 * (45 + 100)");
  auto statements = parser.parse();

  const auto expected =
      FunctionAST(std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string_view>()),
                  std::make_unique<ExprAST>(BinaryExprAST(
                      std::make_unique<ExprAST>(NumberExprAST(55)),
                      TokenType::Star, 
                      std::make_unique<ExprAST>(BinaryExprAST(
                          std::make_unique<ExprAST>(NumberExprAST(45)),
                          TokenType::Plus, 
                          std::make_unique<ExprAST>(NumberExprAST(100)))))));

  ASSERT_EQ(statements.size(), 1);
  ASSERT_EQ(statements[0], expected);
}

TEST(Parser, CallExpr) {
  Parser parser = Parser("func(a,b,100)");
  auto statements = parser.parse();

  std::vector<ExprAST> args;
  args.reserve(3);
  args.emplace_back(VariableExprAST("a"));
  args.emplace_back(VariableExprAST("b"));
  args.emplace_back(NumberExprAST(100));

  const auto expected =
      FunctionAST(std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string_view>()),
                  std::make_unique<ExprAST>(CallExprAST("func", std::move(args))));

  ASSERT_EQ(statements.size(), 1);
  ASSERT_EQ(statements[0], expected);
}

TEST(Parser, Extern) {
  Parser parser = Parser("extern sin(angle)");
  auto statements = parser.parse();

  std::vector<std::string_view> args = {"angle"};

  const auto expected = PrototypeAST("sin", args);

  ASSERT_EQ(statements.size(), 1);
  ASSERT_EQ(statements[0], expected);
}

TEST(Parser, FunctionDefinition) {
  Parser parser = Parser("def func(x,y) x+y");
  auto statements = parser.parse();

  std::vector<std::string_view> args = {"x", "y"};

  const auto expected =
      FunctionAST(std::make_unique<PrototypeAST>("func", args),
                  std::make_unique<ExprAST>(
                      BinaryExprAST(std::make_unique<ExprAST>(VariableExprAST("x")),
                                    TokenType::Plus, 
                                    std::make_unique<ExprAST>(VariableExprAST("y")))));

  ASSERT_EQ(statements.size(), 1);
  ASSERT_EQ(statements[0], expected);
}
