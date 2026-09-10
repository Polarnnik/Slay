#include "slay/font_manager.h"
#include "SDL3_ttf/SDL_ttf.h"

slay::FontManager::~FontManager() {
    for (auto& [key, value] : m_fontMap) {
        TTF_CloseFont(value);
    }
}

bool slay::FontManager::load(const std::string& filePath, const std::string& fontKey) {
    TTF_Font* font = TTF_OpenFont(filePath.c_str(), 16.0f);

    if (!font) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR,
            "Failed to load font '%s': %s",
            filePath.c_str(),
            SDL_GetError()
        );
        return false;
    }

    m_fontMap[fontKey] = font;
    return true;
}

TTF_Font* slay::FontManager::get(const std::string& fontKey) const {
    auto it = m_fontMap.find(fontKey);
    if (it == m_fontMap.end())
        return nullptr;

    return it->second;
}
