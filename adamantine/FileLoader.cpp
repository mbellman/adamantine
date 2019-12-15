#include <fstream>

#include "FileLoader.h"

std::string FileLoader::load(const char* path) {
  std::string source;
  std::ifstream file(path);
  std::string line;

  while (std::getline(file, line)) {
    source.append(line + "\n");
  }

  file.close();

  return source;
}