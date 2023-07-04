#include "TextButton.h"

TextButton::TextButton(sf::Sprite sprite, sf::IntRect rect, sf::Text text, const Animation &hoverAnim,
                       const Animation &pressAnim, const Animation &releaseAnim, std::string hover_s,
                       std::string unhover_s, std::string press_s, std::string release_s, std::function<void()> hover_f,
                       std::function<void()> unhover_f, std::function<void()> press_f, std::function<void()> release_f,
                       std::string font) :
        Button(std::move(sprite), rect,
               hoverAnim, pressAnim, releaseAnim,
               std::move(hover_s), std::move(unhover_s), std::move(press_s), std::move(release_s),
               std::move(hover_f), std::move(unhover_f), std::move(press_f), std::move(release_f)),
        m_text(std::move(text)), m_font(std::move(font)) {
}

void TextButton::standard_text(sf::Text &text) const {
    text.setFont(AssetManager::getFont(m_font));
    text.setCharacterSize(25);
}

void TextButton::hover(const sf::Vector2i &vr) {
    Button::hover(vr);
    if(m_status == 1) m_text.setStyle(sf::Text::Bold);
    else m_text.setStyle(sf::Text::Regular);
}

void TextButton::press(const sf::Vector2i &pos)  {
    if (!contains(pos) || m_status == 2) return;
    Button::press(pos);
    m_text.setFillColor(sf::Color::Red);
}

void TextButton::release(const sf::Vector2i &pos)  {
    if (m_status != 2) return;
    Button::release(pos);
    m_text.setFillColor(sf::Color::White);
}

void TextButton::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(m_sprite);
    target.draw(m_text);
}