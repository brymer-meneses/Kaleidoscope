#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "AST.hpp"

template <typename T>
class ExprASTVisitor {
public:
  virtual T visit(NumberExprAST& node) = 0;
  virtual T visit(VariableExprAST& node) = 0;
  virtual T visit(CallExprAST& node) = 0;
  virtual T visit(BinaryExprAST& node) = 0;
};

template <typename T>
class NodeASTVisitor {
public:
  virtual T visit(PrototypeAST& node) = 0;
  virtual T visit(FunctionAST& node) = 0;
};

#endif
