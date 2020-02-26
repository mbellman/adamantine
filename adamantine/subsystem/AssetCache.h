#pragma once

#include <vector>
#include <map>
#include <string>

#include "subsystem/Texture.h"

class AssetCache {
public:
  ~AssetCache();

  void addTexture(const Texture* texture);
  const Texture* createTexture(std::string path);

private:
  std::map<std::string, const Texture*> textureMap;
};