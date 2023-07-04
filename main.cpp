#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Animator.h"
#include "NumKeyBoard.h"
#include "LCDRowDisplay.h"
#include "UserCursor.h"
#include <utility>
#include <iostream>
#include "AssetManager.h"
#include "TextButton.h"


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
    TextButton tb(sf::Sprite(), {0, 600, 100, 50}, sf::Text(), b1_0, b1_1, b1_2,
                  "media/audio/mouse_hover.ogg", "media/audio/mouse_hover.ogg", "media/audio/click_to_start.ogg", "media/audio/p2_2.wav");
    tb.m_ar.addAnimation(tb.hover_anim);
    tb.standard_text(tb.m_text);
    tb.setPosition(0, 600);
    tb.m_text.setPosition(40, 610);
    tb.m_text.setString(L":)");

    sf::Clock clock;
    sf::Sound sound;
    UserCursor cursor;
    cursor.add(&keyBoard);
    cursor.add(&tb, true, true, false);

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
                    cursor.press(window);
                    break;
                case sf::Event::MouseButtonReleased:
                    cursor.release(window);
                    break;
                case sf::Event::KeyPressed:
                    sound.play();
                    tb.release(tb.m_rect.getPosition());
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