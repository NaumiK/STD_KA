#include <utility>

#include "KA_UI.h"


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

    NumKeyBoard keyBoard(10, 3, {0, 100}, {5, 20}, {100, 50}, {2, 2},
                         b1_0, b1_1, b1_2, "media/audio/p2_1.wav", "media/audio/p2_2.wav", "media/audio/p1_1.wav",
                         "media/audio/p1_2.wav");
    LCDRowDisplay lcdRowDisplay(sf::Sprite(), {}, sf::Text(), "Insert money and choose your drink :)", 16, sf::seconds(0.03), "media/jm.otf");
    lcdRowDisplay.m_text.setPosition(20, 10);
    lcdRowDisplay.m_bck_sprite.setPosition(10, 0);
    lcdRowDisplay.scale({2, 2});
    lcdRowDisplay.standard_user_settings_LCDDisplay(lcdRowDisplay.m_text);
    ChooseDialog cmd({{"50р.", []() { std::cout << 50 << std::endl; }},
                      {"lambda", [&lcdRowDisplay, k{uint64_t(1)}]()mutable {
                          lcdRowDisplay.set_string(std::to_string(k) + "p.");
                          k *= 2;
                      }}},
                     "Вставьте купюру", {600, 0}, {100, 50}, {1, 1});
    TextButton tb(sf::Sprite(), {0, 600, 100, 50}, sf::Text(), {"hover"}, b1_1, b1_2,
                  "media/audio/mouse_hover.ogg", "media/audio/mouse_hover.ogg", "media/audio/buttonclick.ogg",
                  "media/audio/p2_2.wav");
    tb.standard_text(tb.m_text);
    tb.setPosition(0, 600);
    tb.m_text.setPosition(40, 610);
    tb.m_text.setString(L":)");
    MessageBar mb(5, 16, sf::seconds(0.03), {100, 600});
    mb.leave_message(std::string(32, 'a'));
    mb.leave_message(std::string(15, 'b'));

    sf::Clock clock;
    UserCursor cursor;
    cursor.add(&keyBoard);
    cursor.add(&tb, true, true, false);
    cursor.add(&cmd, true, true, true);
    uint64_t k = 0;

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
                    mb.leave_message(std::to_string(k++));
                    break;
                default:
                    break;
            }
        }
        cursor.hover(window);

        window.clear();

        keyBoard.update(dt);
        cmd.update(dt);
        mb.update(dt);
        lcdRowDisplay.update(dt);
        tb.m_ar.update(dt);

        window.draw(keyBoard);
        window.draw(cmd);
        window.draw(mb);
        window.draw(lcdRowDisplay);
        window.draw(tb);
        cursor.draw(window);

        window.display();
    }
    delete am;
    return 0;
}