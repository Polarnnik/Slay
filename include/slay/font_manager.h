#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "ui/types.h"

namespace slay {

class FontManager {
  public:
    FontManager();
    ~FontManager();

    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;

    FontId load(const std::string& filePath);
    TTF_Font* resolve(FontId family, float size);
    void clear();

  private:
    struct Family {
        std::string path;
    };

    std::vector<Family> m_families;
    std::unordered_map<uint64_t, TTF_Font*> m_cache;
};
} // namespace slay
