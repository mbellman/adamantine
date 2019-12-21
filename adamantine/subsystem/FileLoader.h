#pragma once

#include <string>

struct FileLoader {
  static std::string load(const char* path);
};