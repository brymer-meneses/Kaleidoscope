
#include <expected>
#include <iostream>

#include "Kaleidoscope.hpp"

int main(const int argc, const char **argv) {

  Kaleidoscope kaleidoscope = Kaleidoscope(argc, argv);
  kaleidoscope.run();

  return 0;
}
