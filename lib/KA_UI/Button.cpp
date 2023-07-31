#include "Button.h"
#include "AssetManager.h"

#include <utility>
#include <iostream>

Button::Button(sf::Sprite sprite, sf::IntRect rect,
           const Animation &hoverAnim, const Animation &pressAnim, const Animation &releaseAnim,
           std::string hover_s, std::string unhover_s, std::string press_s, std::string release_s,
           std::function<void()> hover_f, std::function<void()> unhover_f,
           std::function<void()> press_f, std::function<void()> release_f)
        : m_sprite(std::move(sprite)), m_ar(m_sprite), m_rect(rect),
          m_hover_s(std::move(hover_s)), m_unhover_s(std::move(unhover_s)), m_press_s(std::move(press_s)), m_release_s(std::move(release_s)),
          m_hover_f(std::move(hover_f)), m_unhover_f(std::move(unhover_f)), m_press_f(std::move(press_f)), m_release_f(std::move(release_f)) {
    auto [x, y] = m_rect.getPosition();
    m_sprite.setPosition((float)x, (float)y);
    m_ar.addAnimation(pressAnim);
    m_ar.addAnimation(hoverAnim);
    m_ar.addAnimation(releaseAnim);
}


void Button::press(const sf::Vector2i &pos) {
    if (!contains(pos) || m_status == 2) return;
    m_clickSound.setBuffer(AssetManager::getSoundBuffer(m_press_s));
    m_clickSound.play();
    m_ar.switchAnimation("press");
    m_ar.restart();
    m_press_f();
    m_status = 2;
}

void Button::release(const sf::Vector2i &pos)  {
    if (m_status != 2) return;
    m_clickSound.setBuffer(AssetManager::getSoundBuffer(m_release_s));
    m_clickSound.play();
    m_ar.switchAnimation("release");
    m_ar.restart();
    m_release_f();
    m_status = 0;
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

bool Button::contains(const sf::Vector2i &vr) const {
    return m_rect.contains(vr);
}

void Button::hover(const sf::Vector2i &vr) {
    if (m_status == 2 || (m_status == 0 && !m_ar.endAnim)) return;
    if (m_status == 0 && contains(vr)) {
        m_ar.switchAnimation("hover");
        m_ar.restart();
        m_status = 1;
        m_clickSound.setBuffer(AssetManager::getSoundBuffer(m_hover_s));
        m_clickSound.play();
        return;
    }
    if (m_status == 1 && !contains(vr)) {
        m_ar.switchAnimation("press");
        m_ar.endAnim = true;
        m_status = 0;
        m_clickSound.setBuffer(AssetManager::getSoundBuffer(m_unhover_s));
        m_clickSound.play();
        return;
    }
}

void Button::update(const sf::Time &dt) {
    m_ar.update(dt);
}
