#include "UserCursor.h"


UserCursor::UserCursor() {
    m_textures[0].first.loadFromFile("media/images/cursor/OpenedArm.png");
    m_textures[1].first.loadFromFile("media/images/cursor/Point.png");
    m_textures[2].first.loadFromFile("media/images/cursor/ClosedArm.png");
}

void UserCursor::draw(sf::RenderWindow &win) {
    auto pos = sf::Mouse::getPosition(win);
    uint64_t mode = 0;
    if (std::ranges::any_of(m_hover_objects, [&pos](auto hvr){return hvr->hover(pos);}))
        mode = 1;
    m_sprite.setTexture(m_textures[mode].first);
    auto [dx, dy] = m_textures[mode].second;
    pos = sf::Mouse::getPosition(win);
    m_sprite.setPosition(static_cast<float>(pos.x + dx), static_cast<float>(pos.y + dy));
    win.draw(m_sprite);
}