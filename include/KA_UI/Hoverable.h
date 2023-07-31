#ifndef STD_PRJ_HOVERABLE_H
#define STD_PRJ_HOVERABLE_H

#include <SFML/System.hpp>

struct Hoverable {
    virtual void hover(const sf::Vector2i &vr) = 0;
    virtual bool contains(const sf::Vector2i &vr) const = 0;
};

struct PressHoverable : public Hoverable {
    virtual void press(const sf::Vector2i &vr) = 0;
    virtual void release(const sf::Vector2i &vr) = 0;
};

struct Updateable {
    virtual void update(const sf::Time &dt) = 0;
};

#endif //STD_PRJ_HOVERABLE_H
