#ifndef STD_PRJ_BUTTON_H
#define STD_PRJ_BUTTON_H


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Animator.h"
#include <functional>

struct Button : public sf::Drawable {
    sf::Sprite m_sprite;
    sf::IntRect m_rect;
    Animator m_ar;
    sf::Sound m_clickSound;
    const sf::SoundBuffer &m_press_s, &m_release_s;
    std::function<void()> m_press_f, m_release_f;
    bool m_is_pressed = false;
    Button(sf::Sprite sprite, sf::IntRect rect,
           Animation &pressAnim, Animation &releaseAnim,
           const sf::SoundBuffer &press_s, const sf::SoundBuffer &release_s,
           std::function<void()> press_f = [](){}, std::function<void()> release_f = [](){});
    void press(const sf::Vector2i &pos);
    void release(const sf::Vector2i &pos);
    void setPosition(int x, int y);
    void setPosition(const sf::Vector2i &pos);
    void scale(const sf::Vector2f &factor);
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //STD_PRJ_BUTTON_H