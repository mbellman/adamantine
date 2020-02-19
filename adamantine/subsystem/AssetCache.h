#pragma once

#include <vector>

#include "subsystem/Texture.h"

class AssetCache {
public:
  ~AssetCache();

  void addTexture(const Texture* texture);
  Texture* createTexture(const char* path);

private:
  std::vector<const Texture*> textures;
};