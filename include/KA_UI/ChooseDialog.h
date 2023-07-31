#ifndef STD_PRJ_CHOOSEDIALOG_H
#define STD_PRJ_CHOOSEDIALOG_H

#include "TextButton.h"
#include "FuncWithDesc.h"

struct ChooseDialog : public sf::Drawable, public PressHoverable, Updateable {
    TextButton m_headline;
    std::list<TextButton> m_options;
    sf::Vector2i m_button_size, m_pos;
    sf::Vector2f m_scale;
    sf::Sound m_speaker;

    void m_func(const std::function<void()>& func);

    ChooseDialog(const std::vector<FuncWithDesc>& options, const std::string &headline, sf::Vector2f pos, sf::Vector2i button_size, sf::Vector2f scale);
    void update(const sf::Time &dt) override;
    bool contains(const sf::Vector2i &vr) const override;

    void hover(const sf::Vector2i &vr) override;
    void press(const sf::Vector2i &vr) override;
    void release(const sf::Vector2i &vr) override;
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif //STD_PRJ_CHOOSEDIALOG_H
