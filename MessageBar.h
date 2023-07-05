#ifndef STD_PRJ_MESSAGEBAR_H
#define STD_PRJ_MESSAGEBAR_H

#include "SFML/Graphics.hpp"
#include "AssetManager.h"

struct MessageBar : sf::Drawable {
private:
    std::vector<std::pair<sf::Text, std::string>> m_lines;
    uint64_t m_message_cnt;
    uint64_t m_length;
    sf::Time m_delta_duration, m_current_time;
    sf::Vector2i m_pos;
    std::string m_font;
    int m_space;
public:
    MessageBar(uint64_t messageCnt, uint64_t length, const sf::Time &dt, const sf::Vector2i &pos, int space = 30,
               std::string font = "media/basis33.ttf");

    void update(sf::Time dt);
    void leave_message(const std::string &message);

private:
    void standard_text(sf::Text &text) const;
    static sf::String strRowWindow(const std::string &str, uint64_t frame, uint64_t l);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //STD_PRJ_MESSAGEBAR_H
