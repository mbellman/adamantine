#pragma once

#include "SDL_image.h"

class Texture {
public:
  Texture(const char* path);
  ~Texture();

  static int total;

  const SDL_Surface* getData() const;
  int getId() const;

private:
  SDL_Surface* surface = nullptr;
  int id;
};