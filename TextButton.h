#ifndef STD_PRJ_TEXTBUTTON_H
#define STD_PRJ_TEXTBUTTON_H

#include "AssetManager.h"
#include "Button.h"

struct TextButton : public Button {
    sf::Text m_text;
    std::string m_font;
    TextButton(sf::Sprite sprite, sf::IntRect rect, sf::Text text,
               const Animation &hoverAnim, const Animation &pressAnim, const Animation &releaseAnim,
               std::string hover_s="", std::string unhover_s="", std::string press_s="", std::string release_s="",
               std::function<void()> hover_f=[](){}, std::function<void()> unhover_f = [](){},
               std::function<void()> press_f = [](){}, std::function<void()> release_f = [](){},
               std::string font="media/EpilepsySans.ttf");
    void standard_text(sf::Text &text) const;
    void hover(const sf::Vector2i &vr) override;
    void press(const sf::Vector2i &pos) override;
    void release(const sf::Vector2i &pos) override;
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //STD_PRJ_TEXTBUTTON_H
