#include "AST.hpp"
#include <iostream>

template <typename T, typename U> 
bool inline isEqual(const T& t1, const T& t2) {
  return static_cast<const U&>(t1) == static_cast<const U&>(t2);
};



bool NumberExprAST::operator==(const NumberExprAST& other) const {
  return other.value == this->value;
}


bool VariableExprAST::operator==(const VariableExprAST& other) const {
  return other.name == this->name;
}

bool CallExprAST::operator==(const CallExprAST& other) const {
  if (other.callee != this->callee)
    return false;

  if (other.args.size() != this->args.size())
    return false;

  for (unsigned int i = 0; i < other.args.size(); i++) {
    if (other.args[i] != this->args[i])
      return false;
  }

  return true;
}

bool BinaryExprAST::operator==(const BinaryExprAST& other) const {

  if (other.op != this->op)
    return false;

  if (*other.left != *this->left) {
    std::cout << "left are not equal" << std::endl;
    return false;
  }


  if (*other.right != *this->right) {
    std::cout << "right are not equal" << std::endl;
    return false;
  }



  return true;
};

bool FunctionAST::operator==(const FunctionAST& other) const {
  if (*other.body != *this->body)
    return false;

  if (*other.proto != *this->proto)
    return false;
  return true;
}

bool PrototypeAST::operator==(const PrototypeAST& other) const {
  if (other.args != this->args)
    return false;

  if (other.name != this->name)
    return false;

  return true;
}
