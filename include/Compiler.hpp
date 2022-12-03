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
#include "KaleidoscopeJIT.hpp"


class Compiler : ExprASTVisitor<llvm::Value*>, NodeASTVisitor<llvm::Function*> {
  using KaleidoscopeJIT = llvm::orc::KaleidoscopeJIT;

private:
  std::unique_ptr<llvm::LLVMContext> mContext;
  std::unique_ptr<llvm::IRBuilder<>> mBuilder;
  std::unique_ptr<llvm::Module> mModule;
  std::unique_ptr<KaleidoscopeJIT> mJIT;
  std::map<std::string_view, llvm::Value*> mNamedValues;
  std::map<std::string_view, std::unique_ptr<PrototypeAST>> mFunctionProtos;
  std::unique_ptr<llvm::legacy::FunctionPassManager> mPassManager;

public:
  Compiler();

  void run(std::vector<NodeAST> nodes);


private:
  void initializeModuleAndPassManager();

  llvm::Function* codegen(NodeAST& node);
  llvm::Value*    codegen(ExprAST& node);

  llvm::Function* getFunction(std::string_view name);

  llvm::Value* visit(BinaryExprAST& node) override;
  llvm::Value* visit(CallExprAST& node) override;
  llvm::Value* visit(VariableExprAST& node) override;
  llvm::Value* visit(NumberExprAST& node) override;

  llvm::Function* visit(PrototypeAST& node) override;
  llvm::Function* visit(FunctionAST& node) override;

  void handleExtern(NodeAST& node);
  void handleFunctionDefinition(NodeAST& node);

  void handleTopLevelExpression(NodeAST& node);
};

#endif
