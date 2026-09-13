#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace slay {

struct FontId {
    uint32_t value = 0;
    explicit operator bool() const { return value != 0; }
};

class FontManager {
  public:
    FontManager();
    ~FontManager();

    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;

    FontId load(const std::string& filePath);
    TTF_Font* resolve(FontId family, float size);

  private:
    struct Family {
        std::string path;
    };

    std::vector<Family> m_families;
    std::unordered_map<uint64_t, TTF_Font*> m_cache;
};
}
