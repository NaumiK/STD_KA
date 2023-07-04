#include "NumKeyBoard.h"
#include "AssetManager.h"

void NumKeyBoard::keySound(uint64_t key, const std::string &filename_prefix) {
    m_speaker.setBuffer(AssetManager::getSoundBuffer(filename_prefix + std::to_string(key) + ".wav"));
    m_speaker.play();
}

sf::Vector2i NumKeyBoard::getPosition(uint64_t k, uint64_t cols,
                                  const sf::Vector2i &distance, const sf::Vector2i &size, const sf::Vector2f &scale) {
    auto w = scale.x * size.x, h = scale.y * size.y;
    auto dw = scale.x * distance.x, dh = scale.y * distance.y;
    return {static_cast<int>((w + dw) * (k % cols)),static_cast<int>((h + dh) * (k / cols)) };
}

NumKeyBoard::NumKeyBoard(uint64_t k, uint64_t cols,
                         const sf::Vector2i &pos0, const sf::Vector2i &distance, const sf::Vector2i &size, const sf::Vector2f &scale,
                         const Animation &hoverAnim, const Animation &pressAnim, const Animation &releaseAnim,
                         const std::string& hover_s, const std::string& unhover_s, const std::string& press_s, const std::string& release_s,
                         const std::string &filename_prefix) {
    auto [x0, y0] = pos0;
    for (size_t i = 0; i < k; ++i) {
        auto [x, y] = getPosition(i - 1, cols, distance, size, scale);
        m_buttons.emplace_back(sf::Sprite(), sf::IntRect({x + x0, y + y0}, size),
                               hoverAnim, pressAnim, releaseAnim,
                               hover_s, unhover_s, press_s, release_s,
                               [](){}, [](){}, [](){}, [&, i, filename_prefix](){
                                   keySound(i, filename_prefix);
                                   keySignal(i);
                               }
        );
        m_buttons.back().m_sprite.setPosition(x + x0, y + y0);
        m_buttons.back().scale(scale);
    }
    auto [x, y] = getPosition(k, cols, distance, size, scale);
    m_buttons.front().setPosition({x0 + x, y0 + y});
}

void NumKeyBoard::press(const sf::Vector2i &pos) {
    for (auto &i: m_buttons)
        i.press(pos);
}

void NumKeyBoard::release(const sf::Vector2i &pos) {
    for (auto &i: m_buttons)
        i.release(pos);
}

void NumKeyBoard::update(const sf::Time &dt) {
    for (auto &i: m_buttons)
        i.m_ar.update(dt);
}

void NumKeyBoard::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &i: m_buttons)
        target.draw(i, states);
}

bool NumKeyBoard::contains(const sf::Vector2i &vr) const {
    return std::ranges::any_of(m_buttons, [&vr](auto &b){return b.m_rect.contains(vr);});
}

void NumKeyBoard::hover(const sf::Vector2i &vr) {
    for (auto &i: m_buttons)
        i.hover(vr);
}
