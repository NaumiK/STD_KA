#ifndef STD_PRJ_LCDROWDISPLAY_H
#define STD_PRJ_LCDROWDISPLAY_H

#include <SFML/Graphics.hpp>
#include "Animator.h"

struct LCDRowDisplay : public sf::Drawable{
    uint64_t m_length;
    sf::Sprite m_bck_sprite;
    Animator m_bck_ar;
    sf::Text m_text;
    sf::Time m_current_time, m_delta_duration;
    std::string m_string, m_font;

    LCDRowDisplay(sf::Sprite bckSprite, const Animation &bckAnim, sf::Text mText,
                  const std::string &string = "", uint64_t length = 16,
                  const sf::Time &deltaDuration = sf::seconds(0.03),
                  std::string font_filename="media/DisplayOTF.otf");
    void set_string(const std::string &string);
    void scale(const sf::Vector2f &factors);
    static sf::String strRowWindow(const std::string &str, uint64_t frame, uint64_t l);
    void standard_user_settings_LCDDisplay(sf::Text &text) const;
    void update(const sf::Time &dt);
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif //STD_PRJ_LCDROWDISPLAY_H
