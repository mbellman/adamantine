#pragma once

#include <string>
#include <vector>

class AbstractLoader {
public:
  virtual ~AbstractLoader() = 0;

protected:
  bool isLoading = false;

  void load(const char* filePath);
  std::string readNextChunk();
  void nextLine();
  void setChunkDelimiter(std::string delimiter);

private:
  std::string buffer = "";
  std::string delimiter = " ";
  FILE* file = 0;

  bool bufferEndsWith(std::string str);
  void fillBufferUntil(std::string end);
  int getDelimiterOffset();
  bool isAtDelimiter();
  bool isAtEOL();
  char nextChar();
};
