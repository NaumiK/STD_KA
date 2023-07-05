#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Animator.h"
#include "NumKeyBoard.h"
#include "LCDRowDisplay.h"
#include "UserCursor.h"
#include <iostream>
#include "AssetManager.h"
#include "TextButton.h"
#include <concepts>
Animation test = {"press", "media/images/b1_1.png",
                  17, 5,
                  {100, 50}, sf::seconds(0.8),
                  {0, 255, 0}, false};

struct FuncWithDesc {
    std::string desc;
    std::function<void()> func;
};

struct ChooseDialog : public sf::Drawable, public PressHoverable {
    TextButton m_headline;
    std::list<TextButton> m_options;
    sf::Vector2i m_button_size, m_pos;
    sf::Vector2f m_scale;
    sf::Sound m_speaker;

    void m_func(const std::function<void()>& func) {
        m_speaker.setBuffer(AssetManager::getSoundBuffer("media/audio/buttonclick.ogg"));
        m_speaker.play();
        func();
    }

    ChooseDialog(const std::vector<FuncWithDesc>& options, const std::string &headline, sf::Vector2f pos, sf::Vector2i button_size, sf::Vector2i scale)
            : m_button_size(button_size), m_scale(scale), m_pos(pos),
              m_headline(sf::Sprite(), {m_pos, m_button_size}, sf::Text(), {}, {}, {})
    {
        m_headline.m_text.setPosition(m_pos.x, m_pos.y);
        m_headline.m_sprite.setPosition(m_pos.x, m_pos.y);
        m_headline.m_rect = {m_pos, m_button_size};
        m_headline.standard_text(m_headline.m_text);
        m_headline.m_text.setString(sf::String::fromUtf8(headline.begin(), headline.end()));
        m_headline.scale(m_scale);

        for (size_t i = 0; i < options.size(); ++i) {
            int x = static_cast<int>(pos.x * m_scale.x), y = static_cast<int>((pos.y + m_button_size.y * (i + 1)) * m_scale.y);
            m_options.emplace_back(sf::Sprite(), sf::IntRect({x, y}, m_button_size),
                                   sf::Text(), Animation("hover"), Animation("press"), Animation("release"),
                                   "media/audio/mouse_hover.ogg", "media/audio/mouse_hover.ogg", "", "",
                                   [](){}, [](){}, [&, option{options[i].func}]()mutable {m_func(std::ref(option));}, [](){});
            m_options.back().m_text.setString(sf::String::fromUtf8(options[i].desc.begin(), options[i].desc.end()));
            m_options.back().m_sprite.setPosition(x, y);
            m_options.back().m_text.setPosition(x, y);
            m_options.back().standard_text(m_options.back().m_text);
        }
    }
    void update(sf::Time dt) {
        m_headline.m_ar.update(dt);
        for (auto &i: m_options) i.m_ar.update(dt);
    }

    bool contains(const sf::Vector2i &vr) const override {
        return std::ranges::any_of(m_options, [&vr](const auto &button){return button.contains(vr);}) || m_headline.contains(vr);
    }

    void hover(const sf::Vector2i &vr) override {
        m_headline.hover(vr);
        for (auto &i: m_options) i.hover(vr);
    }
    void press(const sf::Vector2i &vr) override {
        for (auto &i: m_options) i.press(vr);
    }
    void release(const sf::Vector2i &vr) override {
        for (auto &i: m_options) i.release(vr);
    }
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        target.draw(m_headline, states);
        for (auto &i: m_options) target.draw(i, states);
    }
};

sf::String wrapText(sf::String string, unsigned width, const sf::Font &font, unsigned charicterSize, bool bold){
    unsigned currentOffset = 0;
    bool firstWord = true;
    std::size_t wordBegining = 0;

    for (std::size_t pos(0); pos < string.getSize(); ++pos) {
        auto currentChar = string[pos];
        if (currentChar == '\n'){
            currentOffset = 0;
            firstWord = true;
            continue;
        } else if (currentChar == ' ') {
            wordBegining = pos;
            firstWord = false;
        }

        auto glyph = font.getGlyph(currentChar, charicterSize, bold);
        currentOffset += glyph.advance;

        if (!firstWord && currentOffset > width) {
            pos = wordBegining;
            string[pos] = '\n';
            firstWord = true;
            currentOffset = 0;
        }
    }

    return string;
}


int main() {
    auto am = AssetManager::getInstance();
    Animation b1_1 = {"press", "media/images/b1_1.png",
                      17, 5,
                      {100, 50}, sf::seconds(0.8),
                      {0, 255, 0}, false};
    Animation b1_2 = {"release", "media/images/b1_2.png",
                      9, 3,
                      {100, 50}, sf::seconds(0.4),
                      {0, 255, 0}, false};
    Animation b1_0 = {"hover", "media/images/b1_0.png",
                      9, 3,
                      {100, 50}, sf::seconds(0.4),
                      {0, 255, 0}, true};
    Animation b2_1 = {"press", "media/images/b2_1.png",
                      17, 5,
                      {64, 81}, sf::seconds(0.8),
                      {0, 255, 0}, false};
    Animation b2_2 = {"release", "media/images/b2_2.png",
                      9, 3,
                      sf::Vector2i(64, 81), sf::seconds(0.4),
                      sf::Color(0, 255, 0), false};
    sf::RenderWindow window(sf::VideoMode(800, 800), "Test Manager",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setMouseCursorVisible(false);

    NumKeyBoard keyBoard(10, 3, {0,100}, {5, 20}, {100, 50}, {2, 2},
                         b1_0, b1_1, b1_2, "media/audio/p2_1.wav", "media/audio/p2_2.wav", "media/audio/p1_1.wav", "media/audio/p1_2.wav");
    LCDRowDisplay lcdRowDisplay(sf::Sprite(), {}, sf::Text(), "Insert money and choose your drink :)", 16);
    lcdRowDisplay.m_text.setPosition(20, 10);
    lcdRowDisplay.m_bck_sprite.setPosition(10, 0);
    lcdRowDisplay.scale({2, 2});
    lcdRowDisplay.standard_user_settings_LCDDisplay(lcdRowDisplay.m_text);
    ChooseDialog cmd({{"50р.", [](){std::cout << 50 << std::endl;}},
                      {"lambda", [&lcdRowDisplay, k{uint64_t(1)}]()mutable {lcdRowDisplay.set_string(std::to_string(k) + "p.");k *= 2;}}},
                     "Вставьте купюру", {600, 0}, {100, 50}, {1, 1});
    TextButton tb(sf::Sprite(), {0, 600, 100, 50}, sf::Text(), {"hover"}, b1_1, b1_2,
                  "media/audio/mouse_hover.ogg", "media/audio/mouse_hover.ogg", "media/audio/buttonclick.ogg", "media/audio/p2_2.wav");
    tb.standard_text(tb.m_text);
    tb.setPosition(0, 600);
    tb.m_text.setPosition(40, 610);
    tb.m_text.setString(L":)");

    sf::Clock clock;
    UserCursor cursor;
    cursor.add(&keyBoard);
    cursor.add(&tb, true, true, false);
    cursor.add(&cmd, true, true, true);

    while (window.isOpen()) {
        auto dt = clock.restart();
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    cursor.press(window);
                    break;
                case sf::Event::MouseButtonReleased:
                    cursor.release(window);
                    break;
                case sf::Event::KeyPressed:
                    tb.release(tb.m_rect.getPosition());
                    break;
                default:
                    break;
            }
        }
        cursor.hover(window);

        window.clear();

        keyBoard.update(dt);
        cmd.update(dt);
        lcdRowDisplay.update(dt);
        tb.m_ar.update(dt);

        window.draw(keyBoard);
        window.draw(cmd);
        window.draw(lcdRowDisplay);
        window.draw(tb);
        cursor.draw(window);

        window.display();
    }
    delete am;
    return 0;
}