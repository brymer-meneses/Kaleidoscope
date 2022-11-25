#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <map>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <memory>

#include "Visitor.hpp"


class Compiler : ExprASTVisitor<llvm::Value*>, NodeASTVisitor<llvm::Function*> {

private:
  std::unique_ptr<llvm::LLVMContext> mContext;
  std::unique_ptr<llvm::IRBuilder<>> mBuilder;
  std::unique_ptr<llvm::Module> mModule;
  std::map<std::string_view, llvm::Value*> mNamedValues;

public:
  Compiler() { initializeModule(); };
  llvm::Function* codegen(const NodeAST& node);
  llvm::Value*    codegen(const ExprAST& node);

private:
  void initializeModule() {
    mContext = std::make_unique<llvm::LLVMContext>();
    mModule = std::make_unique<llvm::Module>("__main__", *mContext);
    mBuilder = std::make_unique<llvm::IRBuilder<>>(*mContext);
  }

  llvm::Value* visit(const BinaryExprAST& node) override;
  llvm::Value* visit(const CallExprAST& node) override;
  llvm::Value* visit(const VariableExprAST& node) override;
  llvm::Value* visit(const NumberExprAST& node) override;

  llvm::Function* visit(const PrototypeAST& node) override;
  llvm::Function* visit(const FunctionAST& node) override;

};

#endif
