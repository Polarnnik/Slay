#include "slay/font_manager.h"
#include <SDL3/SDL_log.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <bit>
#include <cstdint>

namespace slay {
namespace {

constexpr float kDefaultSize = 16.0f;

uint64_t cacheKey(uint32_t family, float size) {
    return (static_cast<uint64_t>(family) << 32) | std::bit_cast<uint32_t>(size);
}

}

FontManager::FontManager() {
    m_families.emplace_back();
}

FontManager::~FontManager() {
    for (auto& [key, font] : m_cache) {
        if (font) {
            TTF_CloseFont(font);
        }
    }
}

FontId FontManager::load(const std::string& filePath) {
    TTF_Font* probe = TTF_OpenFont(filePath.c_str(), kDefaultSize);
    if (!probe) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font '%s': %s",
                     filePath.c_str(), SDL_GetError());
        return FontId{};
    }

    FontId id{static_cast<uint32_t>(m_families.size())};
    m_families.push_back({filePath});
    m_cache.emplace(cacheKey(id.value, kDefaultSize), probe);
    return id;
}

TTF_Font* FontManager::resolve(FontId family, float size) {
    if (!family || family.value >= m_families.size() || size <= 0.0f) {
        return nullptr;
    }

    const uint64_t key = cacheKey(family.value, size);
    auto it = m_cache.find(key);
    if (it != m_cache.end()) {
        return it->second;
    }

    TTF_Font* font = TTF_OpenFont(m_families[family.value].path.c_str(), size);
    if (!font) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "Failed to open font '%s' at size %g: %s",
                     m_families[family.value].path.c_str(), size, SDL_GetError());
        return nullptr;
    }

    return m_cache.emplace(key, font).first->second;
}
}
