#include "UserCursor.h"
#include "AssetManager.h"

void UserCursor::draw(sf::RenderWindow &win) {
    auto pos = sf::Mouse::getPosition(win);
    uint64_t mode = 0;
    if (std::ranges::any_of(m_hover_objects, [&pos](auto hvr){return hvr->hover(pos);}))
        mode = 1;
    m_sprite.setTexture(AssetManager::getTexture(m_textures[mode].first));
    auto [dx, dy] = m_textures[mode].second;
    pos = sf::Mouse::getPosition(win);
    m_sprite.setPosition(static_cast<float>(pos.x + dx), static_cast<float>(pos.y + dy));
    win.draw(m_sprite);
}