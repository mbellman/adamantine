#pragma once

#include <vector>

#include "subsystem/Texture.h"

class AssetCache {
public:
  ~AssetCache();

  void addTexture(const Texture* texture);

private:
  std::vector<const Texture*> textures;
};