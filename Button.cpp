#include "Button.h"

#include <utility>

Button::Button(sf::Sprite sprite, sf::IntRect rect, Animation &pressAnim, Animation &releaseAnim,
               const sf::SoundBuffer &press_s, const sf::SoundBuffer &release_s, std::function<void()> press_f,
               std::function<void()> release_f)
   : m_sprite(std::move(sprite)), m_ar(m_sprite),
       m_press_s{press_s}, m_release_s(release_s), m_rect(rect),
       m_press_f(std::move(press_f)), m_release_f(std::move(release_f)) {
    m_ar.addAnimation(pressAnim);
    m_ar.addAnimation(releaseAnim);
}

void Button::press(const sf::Vector2i &pos) {
    if (!m_rect.contains(pos)) return;
    m_clickSound.setBuffer(m_press_s);
    m_clickSound.play();
    m_ar.switchAnimation("press");
    m_ar.restart();
    m_press_f();
    m_is_pressed = true;
}

void Button::release(const sf::Vector2i &pos)  {
    if (!m_is_pressed) return;
    m_clickSound.setBuffer(m_release_s);
    m_clickSound.play();
    m_ar.switchAnimation("release");
    m_ar.restart();
    m_release_f();
    m_is_pressed = false;
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(m_sprite, states);
}

void Button::setPosition(int x, int y) {
    m_sprite.setPosition(x, y);
    m_rect = {{x, y}, m_rect.getSize()};
}

void Button::setPosition(const sf::Vector2i &pos) {
    m_sprite.setPosition(pos.x, pos.y);
    m_rect = {pos, m_rect.getSize()};
}

void Button::scale(const sf::Vector2f &factor) {
    m_sprite.scale(factor);
    auto sz = m_rect.getSize();
    m_rect = {m_rect.getPosition(), {static_cast<int>(sz.x * factor.x), static_cast<int>(sz.y * factor.y)}};
}

