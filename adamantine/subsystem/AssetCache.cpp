#include "subsystem/AssetCache.h"

AssetCache::~AssetCache() {
  for (auto& [key, texture] : textureMap) {
    delete texture;
  }

  textureMap.clear();
}

void AssetCache::addTexture(const Texture* texture) {
  textureMap.emplace(texture->getPath(), texture);
}

const Texture* AssetCache::createTexture(std::string path) {
  if (textureMap.find(path) != textureMap.end()) {
    return textureMap.at(path);
  }

  auto* texture = new Texture(path);

  addTexture(texture);

  return texture;
}