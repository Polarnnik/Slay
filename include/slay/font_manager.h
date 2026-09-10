#pragma once

#include "SDL3_ttf/SDL_ttf.h"
#include <string>
#include <unordered_map>

namespace slay {

class FontManager {
  public:
    ~FontManager();
    bool load(const std::string& filePath, const std::string& fontKey);
    TTF_Font* get(const std::string& fontKey) const;
  private:
    std::unordered_map<std::string, TTF_Font*> m_fontMap;
};
}
