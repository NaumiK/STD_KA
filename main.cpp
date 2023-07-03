#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Animator.h"
#include "NumKeyBoard.h"
#include "LCDRowDisplay.h"
#include "UserCursor.h"
#include <utility>
#include "AssetManager.h"

struct TextButton : public Button {
    sf::Text m_text;
    std::string m_font;
    TextButton(sf::Sprite sprite, sf::IntRect rect, sf::Text text,
               Animation &pressAnim, Animation &releaseAnim,
               std::string press_s, std::string release_s,
               std::function<void()> press_f = []() {}, std::function<void()> release_f = []() {},
               std::string  font="media/EpilepsySans.ttf") :
                       Button(std::move(sprite), rect,
                              pressAnim, releaseAnim,
                              std::move(press_s), std::move(release_s),
                              std::move(press_f), std::move(release_f)),
                              m_text(std::move(text)), m_font(std::move(font)) {
    }
    void standard_text(sf::Text &text) const {
        text.setFont(AssetManager::getFont(m_font));
        text.setCharacterSize(25);
    }
    void hover(const sf::Vector2i &vr) override {
        if (contains(vr))
            m_text.setStyle(sf::Text::Bold);
        else
            m_text.setStyle(sf::Text::Regular);
    }
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        target.draw(m_sprite);
        target.draw(m_text);
    }
};

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
                         b1_1, b1_2, "media/audio/p1_1.wav", "media/audio/p1_2.wav");
    LCDRowDisplay lcdRowDisplay(sf::Sprite(), {}, sf::Text(), "Insert money and choose your drink :)", 16);
    lcdRowDisplay.m_text.setPosition(20, 10);
    lcdRowDisplay.m_bck_sprite.setPosition(10, 0);
    lcdRowDisplay.scale({2, 2});
    lcdRowDisplay.standard_user_settings_LCDDisplay(lcdRowDisplay.m_text);
    TextButton tb(sf::Sprite(), {0, 600, 100, 50}, sf::Text(), b1_1, b1_2, "media/audio/p2_1.wav", "media/audio/p2_2.wav");
    tb.standard_text(tb.m_text);
    tb.setPosition(0, 600);
    tb.m_text.setPosition(40, 610);
    tb.m_text.setString(L":)");

    sf::Clock clock;
    sf::Sound sound;
    UserCursor cursor;
    cursor.m_hover_objects.push_back(&keyBoard);
    cursor.m_hover_objects.push_back(&tb);

    sound.setBuffer(AssetManager::getSoundBuffer("media/audio/done.wav"));
    while (window.isOpen()) {
        auto dt = clock.restart();
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    keyBoard.press(sf::Mouse::getPosition(window));
                    tb.press(sf::Mouse::getPosition(window));
                    break;
                case sf::Event::MouseButtonReleased:
                    keyBoard.release(sf::Mouse::getPosition(window));
                    tb.release(sf::Mouse::getPosition(window));
                    break;
                case sf::Event::KeyPressed:
                    sound.play();
                    break;
                default:
                    break;
            }
        }
        cursor.hover(window);

        window.clear();

        keyBoard.update(dt);
        lcdRowDisplay.update(dt);
        tb.m_ar.update(dt);

        window.draw(keyBoard);
        window.draw(lcdRowDisplay);
        window.draw(tb);
        cursor.draw(window);

        window.display();
    }
    delete am;
    return 0;
}