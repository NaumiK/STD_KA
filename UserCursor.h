#ifndef STD_PRJ_USERCURSOR_H
#define STD_PRJ_USERCURSOR_H

#include <SFML/Graphics.hpp>
#include "Hoverable.h"
#include <list>

struct UserCursor {
    sf::Sprite m_sprite;
    std::vector<std::pair<std::string, sf::Vector2i>> m_textures = {
            {"media/images/cursor/OpenedArm.png", {-25, -25}},
            {"media/images/cursor/Point.png", {-18, -12}},
            {"media/images/cursor/ClosedArm.png", {-25, -25}}
    };
    std::list<Hoverable *> m_hover_objects;
    std::list<PressHoverable *> m_press_objects;
    std::list<PressHoverable *> m_release_objects;

    UserCursor() = default;

    void hover(sf::RenderWindow &win);
    void press(sf::RenderWindow &win);
    void release(sf::RenderWindow &win);

    void add(PressHoverable *ph, bool hover = true, bool press = true, bool release = true);

    void draw(sf::RenderWindow &win);
};

#endif //STD_PRJ_USERCURSOR_H
