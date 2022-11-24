#ifndef KALEIDOSCOPE_HPP
#define KALEIDOSCOPE_HPP

#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>

class Kaleidoscope {
private:
  bool hadError;
  const int argc;
  const char **argv;

public:
  Kaleidoscope(const int argc, const char **argv)
      : hadError(false), argc(argc), argv(argv){};

  void run();

private:
  void runPrompt();
  void execute(std::string_view source);
  void runFile(std::string_view path);
};

#endif // !KALEIDOSCOPE_HPP
