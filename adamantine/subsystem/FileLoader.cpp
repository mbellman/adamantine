#include <fstream>

#include "subsystem/FileLoader.h"

std::string FileLoader::load(const char* path) {
  std::string source;
  std::ifstream file(path);

  if (file.fail()) {
    printf("Failed to load file: %s\n", path);
  } else {
    std::string line;

    while (std::getline(file, line)) {
      source.append(line + "\n");
    }
  }

  file.close();

  return source;
}