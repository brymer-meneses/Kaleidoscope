#ifndef ERROR_HPP
#define ERROR_HPP

#include "Location.hpp"
#include <cassert>
#include <iostream>
#include <optional>
#include <string>

struct Error {
  std::string_view message;
  std::optional<std::string_view> tip;
  std::optional<FileLoc> fileLoc;
  std::optional<LineLoc> lineLoc;

  Error(std::string_view message)
      : message(message), tip(std::nullopt), fileLoc(std::nullopt),
        lineLoc(std::nullopt) {}

  Error &setTip(std::string_view tip) {
    this->tip = tip;
    return *this;
  }

  Error &setFileLoc(FileLoc fileLoc) {
    assert(!lineLoc.has_value());

    this->fileLoc = fileLoc;
    return *this;
  }

  Error &setLineLoc(std::size_t start, std::size_t end, std::size_t line) {
    assert(!fileLoc.has_value());

    this->lineLoc = LineLoc(start, end, line);
    return *this;
  }

  virtual ~Error() = default;
  virtual void report() {}
};

#endif
