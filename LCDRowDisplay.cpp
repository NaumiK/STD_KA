#include "LCDRowDisplay.h"

#include <utility>
#include "AssetManager.h"


LCDRowDisplay::LCDRowDisplay(sf::Sprite bckSprite, const Animation &bckAnim, sf::Text mText, const std::string &string,
                             uint64_t length, const sf::Time &deltaDuration, std::string font_filename)
        : m_length(length),
          m_bck_sprite(std::move(bckSprite)), m_bck_ar(m_bck_sprite),
          m_text(std::move(mText)), m_font(std::move(font_filename)),
          m_delta_duration(deltaDuration) {
    set_string(string);
    m_bck_ar.addAnimation(bckAnim);
}

void LCDRowDisplay::set_string(const std::string &string){
    m_string = std::string(m_length, ' ') + string + std::string(m_length, ' ');
    m_current_time = sf::Time::Zero;
}

void LCDRowDisplay::scale(const sf::Vector2f &factors)  {
    m_text.scale(factors);
    m_bck_sprite.scale(factors);
}

sf::String LCDRowDisplay::strRowWindow(const std::string &str, uint64_t frame, uint64_t l) {
    if (l >= str.length()) return sf::String::fromUtf8(str.begin(), str.end());
    frame = frame % (str.length() - l + 1);
    return sf::String::fromUtf8(str.begin() + (int) frame, str.begin() + (int) (frame + l));
}

void LCDRowDisplay::standard_user_settings_LCDDisplay(sf::Text &text) const {
    text.setFont(AssetManager::getFont(m_font));
    text.setCharacterSize(25);
    text.setLetterSpacing(2);
}

void LCDRowDisplay::update(const sf::Time &dt) {
    m_bck_ar.update(dt);
    m_current_time += dt;
    float scaledTime = (m_current_time.asSeconds() / (m_delta_duration.asSeconds() *
                                                      static_cast<float>(m_string.length())));
    auto cur_frame = static_cast<uint64_t>(scaledTime * static_cast<float>(m_length));
    m_text.setString(strRowWindow(m_string, cur_frame, m_length));
}

void LCDRowDisplay::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(m_bck_sprite, states);
    target.draw(m_text, states);
}