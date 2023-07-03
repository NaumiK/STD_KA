#ifndef STD_PRJ_HOVERABLE_H
#define STD_PRJ_HOVERABLE_H

#include <SFML/System.hpp>

struct Hoverable {
    virtual bool hover(const sf::Vector2i &vr) const = 0;
};

#endif //STD_PRJ_HOVERABLE_H