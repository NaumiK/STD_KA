#include "AssetManager.h"

sf::Texture &AssetManager::getTexture(const std::string &filename, const sf::Color &clr) {
    auto &texMap = assetManager_->m_Textures;
    auto pairFound = texMap.find(filename);
    if (pairFound != texMap.end())
        return pairFound->second;
    else {
        auto &texture = texMap[filename];
        sf::Image img;
        img.loadFromFile(filename);
        img.createMaskFromColor(clr);
        texture.loadFromImage(img);
        return texture;
    }
}

sf::SoundBuffer &AssetManager::getSoundBuffer(const std::string &filename) {
    auto &texMap = assetManager_->m_SoundBuffers;
    auto pairFound = texMap.find(filename);
    if (pairFound != texMap.end())
        return pairFound->second;
    else {
        auto &sb = texMap[filename];
        sb.loadFromFile(filename);
        return sb;
    }
}

sf::Font &AssetManager::getFont(const std::string &filename) {
    auto &texMap = assetManager_->m_Fonts;
    auto pairFound = texMap.find(filename);
    if (pairFound != texMap.end())
        return pairFound->second;
    else {
        auto &font = texMap[filename];
        font.loadFromFile(filename);
        return font;
    }
}

AssetManager *AssetManager::getInstance() {
    if (assetManager_ == nullptr)
        assetManager_ = new AssetManager;
    return assetManager_;
}