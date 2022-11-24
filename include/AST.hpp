#ifndef AST_HPP
#define AST_HPP

#include "Token.hpp"

#include <memory>
#include <string>
#include <vector>

struct NumberExprAST;
struct VariableExprAST;
struct CallExprAST;
struct BinaryExprAST;

struct FunctionAST;
struct PrototypeAST;

using ExprAST = std::variant<NumberExprAST, VariableExprAST, BinaryExprAST, CallExprAST>;
using NodeAST = std::variant<ExprAST, FunctionAST, PrototypeAST>;


struct NumberExprAST {
  double value;
  NumberExprAST(double value) : value(value) {}

  bool operator==(const NumberExprAST& other) const;
};

struct VariableExprAST {
  std::string_view name;

  VariableExprAST(const std::string_view& name) : name(name) {}

  bool operator==(const VariableExprAST& other) const;
};

struct BinaryExprAST {
  TokenType op;
  std::unique_ptr<ExprAST> left, right;

  BinaryExprAST(std::unique_ptr<ExprAST> left, TokenType op, std::unique_ptr<ExprAST> right)
      : op(op), left(std::move(left)), right(std::move(right)) {}

  bool operator==(const BinaryExprAST& other) const;
};

struct CallExprAST {
  std::string_view callee;
  std::vector<ExprAST> args;

  CallExprAST(const std::string_view& callee, std::vector<ExprAST> args)
      : callee(callee), args(std::move(args)) {}

  bool operator==(const CallExprAST& other) const;
};


struct PrototypeAST {
  std::string_view name;
  std::vector<std::string_view> args;

  PrototypeAST(const std::string_view& name, std::vector<std::string_view> args)
      : name(name), args(std::move(args)) {}

  bool operator==(const PrototypeAST& other) const;
};


struct FunctionAST {
  std::unique_ptr<PrototypeAST> proto;
  std::unique_ptr<ExprAST> body;

  FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body)
      : proto(std::move(proto)), body(std::move(body)) {}

  bool operator==(const FunctionAST& other) const;
};

template<typename T, class... Types>
inline bool operator==(const T& t, const std::variant<Types...>& v) {
    const T* c = std::get_if<T>(&v);
    if (c)
        return *c == t;
    else
        return false;
}

template<typename T, class... Types>
inline bool operator==(const std::variant<Types...>& v, const T& t) {
    return t == v;
}

#endif // !AST_HPP
