#ifndef STD_PRJ_BUTTON_H
#define STD_PRJ_BUTTON_H


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Animator.h"
#include <functional>
#include "Hoverable.h"

struct Button : public sf::Drawable, public PressHoverable, public Updateable {
    sf::Sprite m_sprite;
    sf::IntRect m_rect;
    Animator m_ar;
    sf::Sound m_clickSound;
    std::string m_hover_s, m_unhover_s, m_press_s, m_release_s;
    std::function<void()> m_hover_f, m_unhover_f, m_press_f, m_release_f;
    uint8_t m_status = 0;
    Button(sf::Sprite sprite, sf::IntRect rect,
           const Animation &hoverAnim, const Animation &pressAnim, const Animation &releaseAnim,
           std::string hover_s="", std::string unhover_s = "", std::string press_s="", std::string release_s="",
           std::function<void()> hover_f = [](){}, std::function<void()> unhover_f=[](){},
           std::function<void()> press_f = [](){}, std::function<void()> release_f = [](){});
    void press(const sf::Vector2i &pos) override;
    void release(const sf::Vector2i &pos) override;
    void setPosition(int x, int y);
    void setPosition(const sf::Vector2i &pos);
    void scale(const sf::Vector2f &factor);
    bool contains(const sf::Vector2i &vr) const override;
    void hover(const sf::Vector2i &vr) override;
    void update(const sf::Time &dt) override;
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //STD_PRJ_BUTTON_H
