#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdio>
#include <iostream>

inline void LogError(const char* string) {
  fprintf(stderr, "LogError: %s\n", string);
}

#endif // !LOGGER_HPP
