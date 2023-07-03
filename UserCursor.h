#ifndef STD_PRJ_USERCURSOR_H
#define STD_PRJ_USERCURSOR_H
#include <SFML/Graphics.hpp>
#include "Hoverable.h"
#include <list>

struct UserCursor  {
    sf::Sprite m_sprite;
    std::vector<std::pair<sf::Texture, sf::Vector2i>> m_textures = { {sf::Texture(), {-25, -25}},
                                                                     {sf::Texture(), {-18, -12}},
                                                                     {sf::Texture(), {-25, -25}}};
    std::list<const Hoverable*> m_hover_objects;
    UserCursor();
    void draw(sf::RenderWindow &win);
};

#endif //STD_PRJ_USERCURSOR_H
