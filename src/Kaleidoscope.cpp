#include <iostream>
#include <string>

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
  // Parser parser = Parser(std::move(source));
  // parser.parse();
}
