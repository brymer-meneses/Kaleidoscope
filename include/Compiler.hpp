#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Scalar.h>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "Visitor.hpp"


class Compiler : ExprASTVisitor<llvm::Value*>, NodeASTVisitor<llvm::Function*> {

private:
  std::unique_ptr<llvm::LLVMContext> mContext;
  std::unique_ptr<llvm::IRBuilder<>> mBuilder;
  std::unique_ptr<llvm::Module> mModule;
  std::map<std::string_view, llvm::Value*> mNamedValues;
  std::unique_ptr<llvm::legacy::FunctionPassManager> mPassManager;

public:
  Compiler() { 
    initializeModule(); 
    initializePassManager();
  };

  llvm::Function* codegen(const NodeAST& node);
  llvm::Value*    codegen(const ExprAST& node);

private:
  void initializeModule();
  void initializePassManager();


  llvm::Value* visit(const BinaryExprAST& node) override;
  llvm::Value* visit(const CallExprAST& node) override;
  llvm::Value* visit(const VariableExprAST& node) override;
  llvm::Value* visit(const NumberExprAST& node) override;

  llvm::Function* visit(const PrototypeAST& node) override;
  llvm::Function* visit(const FunctionAST& node) override;

};

#endif
