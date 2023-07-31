#ifndef STD_PRJ_STATUS_H
#define STD_PRJ_STATUS_H

#include <list>
#include <SFML/Graphics.hpp>
#include "Hoverable.h"

struct Status {
    std::list<std::shared_ptr<Hoverable>> m_hover_objects;
    std::list<std::shared_ptr<PressHoverable>> m_press_objects;
    std::list<std::shared_ptr<PressHoverable>> m_release_objects;
    std::list<std::shared_ptr<Updateable>> m_update_objects;
    std::list<std::shared_ptr<sf::Drawable>> m_draw_objects;
};

#endif //STD_PRJ_STATUS_H
