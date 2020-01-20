#include <cstdio>

#include "SDL_image.h"
#include "subsystem/Texture.h"

Texture::Texture(const char* path) {
  id = Texture::total++;
  surface = IMG_Load(path);

  if (!surface) {
    printf("Failed to load texture: %s\n", path);
  }
}

Texture::~Texture() {
  SDL_FreeSurface(surface);
}

const SDL_Surface* Texture::getData() const {
  return surface;
}

int Texture::getId() const {
  return id;
}

int Texture::total = 0;