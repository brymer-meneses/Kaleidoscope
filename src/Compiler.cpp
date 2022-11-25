#include "Compiler.hpp"
#include "AST.hpp"

#include <iostream>
#include <cstdio>
#include <memory>
#include <utility>
#include <variant>

void Compiler::initializeModule() {
  mContext = std::make_unique<llvm::LLVMContext>();
  mModule = std::make_unique<llvm::Module>("__main__", *mContext);
  mBuilder = std::make_unique<llvm::IRBuilder<>>(*mContext);
}

void Compiler::initializePassManager() {
  mPassManager = std::make_unique<llvm::legacy::FunctionPassManager>(mModule.get());

  mPassManager->add(llvm::createInstructionCombiningPass());
  mPassManager->add(llvm::createReassociatePass());
  mPassManager->add(llvm::createGVNPass());
  mPassManager->add(llvm::createCFGSimplificationPass());
  mPassManager->doInitialization();
}

llvm::Value* Compiler::codegen(const ExprAST& node) {

  if (std::holds_alternative<NumberExprAST>(node)) {
    return visit(std::get<NumberExprAST>(node));

  } else if (std::holds_alternative<BinaryExprAST>(node)) {
    return visit(std::get<BinaryExprAST>(node));

  } else if (std::holds_alternative<CallExprAST>(node)) {
    return visit(std::get<CallExprAST>(node));

  } else if (std::holds_alternative<VariableExprAST>(node)) {
    return visit(std::get<VariableExprAST>(node));
  };

  return nullptr;
}
 
llvm::Function* Compiler::codegen(const NodeAST& node) {

  if (std::holds_alternative<ExprAST>(node)) {
    codegen(std::get<ExprAST>(node));
    return nullptr;

  } else if (std::holds_alternative<FunctionAST>(node)) {
    return visit(std::get<FunctionAST>(node));

  } else if (std::holds_alternative<PrototypeAST>(node)) {
    return visit(std::get<PrototypeAST>(node));
  };


  return nullptr;
}

llvm::Function* Compiler::visit(const FunctionAST& node) {

  llvm::Function* function = mModule->getFunction(node.proto->name);

  if (!function)
    function = codegen(*node.proto);
  if (!function)
    return nullptr;

  llvm::BasicBlock* basicBlock = llvm::BasicBlock::Create(*mContext, "entry", function);
  mBuilder->SetInsertPoint(basicBlock);

  mNamedValues.clear();
  for (auto& arg : function->args()) {
    mNamedValues[arg.getName()] = &arg;
  }

  llvm::Value* returnValue = codegen(*node.body);

  if (returnValue) {
    mBuilder->CreateRet(returnValue);
    llvm::verifyFunction(*function);

    // Run the optimizer on the function
    mPassManager->run(*function);
    return function;
  };

  function->eraseFromParent();
  return nullptr;
}

llvm::Function* Compiler::visit(const PrototypeAST& node) {
  std::vector<llvm::Type*> doubles(node.args.size(), llvm::Type::getDoubleTy(*mContext));
  llvm::FunctionType *functionType = llvm::FunctionType::get(llvm::Type::getDoubleTy(*mContext), doubles, false);


  llvm::Function* func = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, node.name, *mModule);

  unsigned idx = 0;
  for (auto &arg : func->args()) {
    arg.setName(node.args[idx]);
    idx += 1;
  };

  return func;
}

llvm::Value* Compiler::visit(const NumberExprAST& node) {
  return llvm::ConstantFP::get(*mContext, llvm::APFloat(node.value));
};

llvm::Value* Compiler::visit(const VariableExprAST& node) {
  llvm::Value* value  = mNamedValues[node.name];
  if (!value) {
    std::cout << "variable " << node.name << "not found" << std::endl;
    return nullptr;
  }
  return value;
};

llvm::Value* Compiler::visit(const BinaryExprAST& node) {

  llvm::Value* left  = codegen(*node.left);
  llvm::Value* right = codegen(*node.right);

  if (!left || !right)
    return nullptr;
  switch (node.op) {
    case TokenType::Plus:
      return mBuilder->CreateFAdd(left, right,  "addtmp");
    case TokenType::Minus:
      return mBuilder->CreateFSub(left, right,  "subtmp");
    case TokenType::Star:
      return mBuilder->CreateFMul(left, right,  "multmp");
    case TokenType::Lesser:
      left = mBuilder->CreateFCmpULT(left, right, "cmptmp");
      return mBuilder->CreateUIToFP(left, llvm::Type::getDoubleTy(*mContext), "cmptmp");
    case TokenType::LesserEqual:
      left = mBuilder->CreateFCmpULE(left, right, "cmptmp");
      return mBuilder->CreateUIToFP(left, llvm::Type::getDoubleTy(*mContext), "cmptmp");
    case TokenType::Greater:
      left = mBuilder->CreateFCmpUGT(left, right, "cmptmp");
      return mBuilder->CreateUIToFP(left, llvm::Type::getDoubleTy(*mContext), "cmptmp");
    case TokenType::GreaterEqual:
      left = mBuilder->CreateFCmpUGT(left, right, "cmptmp");
      return mBuilder->CreateUIToFP(left, llvm::Type::getDoubleTy(*mContext), "cmptmp");
    default:
      std::cout << "Invalid binary operation\n";
      return nullptr;
  }
}

llvm::Value* Compiler::visit(const CallExprAST& node) {
  llvm::Function* calleeFunc = mModule->getFunction(node.callee);
  if (!calleeFunc) {
    puts("Unknown function referenced");
    return nullptr;
  }

  if (calleeFunc->arg_size() != node.args.size()) {
    puts("Incorrect number of arguments passed");
    return nullptr;
  }

  std::vector<llvm::Value*> argsV;

  for (unsigned i=0; i< node.args.size(); i++) {
    argsV.emplace_back(codegen(node.args[i]));
    if (!argsV.back()) 
      return nullptr;
  }

  return mBuilder->CreateCall(calleeFunc, argsV, "calltmp");
}
