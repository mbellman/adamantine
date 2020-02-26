#include <cstdio>
#include <string>

#include "SDL_image.h"
#include "subsystem/Texture.h"

Texture::Texture(std::string path) {
  this->path = path;

  id = Texture::total++;
  surface = IMG_Load(path.c_str());

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

const std::string& Texture::getPath() const {
  return path;
}

int Texture::getId() const {
  return id;
}

int Texture::total = 0;