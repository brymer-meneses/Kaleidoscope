#include <iostream>
#include <string>
#include <type_traits>

#include "Kaleidoscope.hpp"
#include "Parser.hpp"

void Kaleidoscope::run() {
  if (this->argc == 1) {
    runPrompt();
    return;
  };
  runFile(this->argv[1]);
};

void Kaleidoscope::runFile(std::string_view path) { std::cout << path << std::endl; }

void Kaleidoscope::runPrompt() {

  std::string inputLine;

  while (true) {
    std::cout << ">>> ";
    std::getline(std::cin, inputLine);
    if (inputLine.empty())
      break;
    execute(inputLine);
  }
}

void Kaleidoscope::execute(std::string_view source) {
  Parser parser = Parser(source);
  auto statements = parser.parse();

  for (NodeAST& statement : statements) {
    auto* IR = mCompiler.codegen(statement);
    if (!IR)
      return;

    IR->print(llvm::errs());
    fprintf(stderr, "\n");


    if (std::holds_alternative<FunctionAST>(statement)) {
      if (std::get<FunctionAST>(statement).proto->name == "__anon_expr")
        IR->eraseFromParent();
    }

  }

}
