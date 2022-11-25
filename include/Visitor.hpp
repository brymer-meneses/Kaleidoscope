#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "AST.hpp"

template <typename T>
class ExprASTVisitor {
public:
  virtual T visit(const NumberExprAST& node) = 0;
  virtual T visit(const VariableExprAST& node) = 0;
  virtual T visit(const CallExprAST& node) = 0;
  virtual T visit(const BinaryExprAST& node) = 0;
};

template <typename T>
class NodeASTVisitor {
public:
  virtual T visit(const PrototypeAST& node) = 0;
  virtual T visit(const FunctionAST& node) = 0;
};

#endif
