#include "ChooseDialog.h"

void ChooseDialog::m_func(const std::function<void()> &func) {
    m_speaker.setBuffer(AssetManager::getSoundBuffer("media/audio/buttonclick.ogg"));
    m_speaker.play();
    func();
}

ChooseDialog::ChooseDialog(const std::vector<FuncWithDesc> &options, const std::string &headline, sf::Vector2f pos,
                           sf::Vector2i button_size, sf::Vector2i scale)
        : m_button_size(button_size), m_scale(scale), m_pos(pos),
          m_headline(sf::Sprite(), {m_pos, m_button_size}, sf::Text(), {}, {}, {}) {
    m_headline.m_text.setPosition(m_pos.x, m_pos.y);
    m_headline.m_sprite.setPosition(m_pos.x, m_pos.y);
    m_headline.m_rect = {m_pos, m_button_size};
    m_headline.standard_text(m_headline.m_text);
    m_headline.m_text.setString(sf::String::fromUtf8(headline.begin(), headline.end()));
    m_headline.scale(m_scale);

    for (size_t i = 0; i < options.size(); ++i) {
        int x = static_cast<int>(pos.x * m_scale.x), y = static_cast<int>((pos.y + m_button_size.y * (i + 1)) *
                                                                          m_scale.y);
        m_options.emplace_back(sf::Sprite(), sf::IntRect({x, y}, m_button_size),
                               sf::Text(), Animation("hover"), Animation("press"), Animation("release"),
                               "media/audio/mouse_hover.ogg", "media/audio/mouse_hover.ogg", "", "",
                               []() {}, []() {}, [&, option{options[i].func}]()mutable { m_func(std::ref(option)); },
                               []() {});
        m_options.back().m_text.setString(sf::String::fromUtf8(options[i].desc.begin(), options[i].desc.end()));
        m_options.back().m_sprite.setPosition(x, y);
        m_options.back().m_text.setPosition(x, y);
        m_options.back().standard_text(m_options.back().m_text);
    }
}

void ChooseDialog::update(sf::Time dt) {
    m_headline.m_ar.update(dt);
    for (auto &i: m_options) i.m_ar.update(dt);
}

bool ChooseDialog::contains(const sf::Vector2i &vr) const {
    return std::ranges::any_of(m_options, [&vr](const auto &button){return button.contains(vr);}) || m_headline.contains(vr);
}

void ChooseDialog::hover(const sf::Vector2i &vr) {
    m_headline.hover(vr);
    for (auto &i: m_options) i.hover(vr);
}

void ChooseDialog::press(const sf::Vector2i &vr) {
    for (auto &i: m_options) i.press(vr);
}

void ChooseDialog::release(const sf::Vector2i &vr) {
    for (auto &i: m_options) i.release(vr);
}

void ChooseDialog::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(m_headline, states);
    for (auto &i: m_options) target.draw(i, states);
}