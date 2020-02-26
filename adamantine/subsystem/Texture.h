#pragma once

#include <string>

#include "SDL_image.h"

class Texture {
public:
  Texture(std::string path);
  ~Texture();

  static int total;

  const SDL_Surface* getData() const;
  const std::string& getPath() const;
  int getId() const;

private:
  std::string path;
  SDL_Surface* surface = nullptr;
  int id;
};