#include "subsystem/AssetCache.h"

AssetCache::~AssetCache() {
  for (auto* texture : textures) {
    delete texture;
  }

  textures.clear();
}

void AssetCache::addTexture(const Texture* texture) {
  textures.push_back(texture);
}