#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <cstddef>
#include <vector>

struct LineLoc {
  // starting position at the line
  std::size_t start;
  // ending position at the line
  std::size_t end;
  // line number
  std::size_t line;

  LineLoc(std::size_t start, std::size_t end, std::size_t line)
      : start(start), end(end), line(line) {}
};

using FileLoc = std::vector<LineLoc>;

#endif // !FILELOC_HPP
