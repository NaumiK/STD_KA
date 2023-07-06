#include "UserCursor.h"
#include "AssetManager.h"

void UserCursor::draw(sf::RenderWindow &win) {
    auto pos = sf::Mouse::getPosition(win);
    uint64_t mode = 0;
    if (std::ranges::any_of(m_hover_objects, [&pos](auto hvr){return hvr->contains(pos);}))
        mode = 1;
    m_sprite.setTexture(AssetManager::getTexture(m_textures[mode].first));
    auto [dx, dy] = m_textures[mode].second;
    pos = sf::Mouse::getPosition(win);
    m_sprite.setPosition(static_cast<float>(pos.x + dx), static_cast<float>(pos.y + dy));
    win.draw(m_sprite);
}

void UserCursor::hover(sf::RenderWindow &win) {
    auto pos = sf::Mouse::getPosition(win);
    for (auto &i: m_hover_objects)
        i->hover(pos);
}

void UserCursor::press(sf::RenderWindow &win) {
    auto pos = sf::Mouse::getPosition(win);
    for (auto &i: m_press_objects)
        i->press(pos);
}

void UserCursor::release(sf::RenderWindow &win) {
    auto pos = sf::Mouse::getPosition(win);
    for (auto &i: m_release_objects)
        i->release(pos);
}

void UserCursor::add(std::shared_ptr<PressHoverable> ph, bool h, bool p, bool r) {
    if (h) m_hover_objects.push_back(ph);
    if (p) m_press_objects.push_back(ph);
    if (r) m_release_objects.push_back(ph);
}
