#ifndef STD_PRJ_ASSETMANAGER_H
#define STD_PRJ_ASSETMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>

struct AssetManager {
protected:
    std::map<std::string, sf::Texture> m_Textures;
    std::map<std::string, sf::SoundBuffer> m_SoundBuffers;
    std::map<std::string, sf::Font> m_Fonts;
protected:
    AssetManager() = default;
    inline static AssetManager *assetManager_ = nullptr;
public:
    AssetManager(AssetManager &other) = delete;
    void operator=(const AssetManager &) = delete;

    static AssetManager *getInstance();

    static sf::Texture &getTexture(std::string const &filename, const sf::Color &clr={0, 255, 0});
    static sf::SoundBuffer &getSoundBuffer(std::string const &filename);
    static sf::Font &getFont(std::string const &filename);
};


#endif //STD_PRJ_ASSETMANAGER_H
