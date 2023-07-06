#include <utility>

#include "KA_UI.h"
#include "Bank.h"
#include "Status.h"

struct STD {
    std::unique_ptr<sf::RenderWindow> m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1600, 550),
                                                                                    "Test Manager",
                                                                                    sf::Style::Titlebar |
                                                                                    sf::Style::Close);
    std::map<std::string, Status> m_status;
    AssetManager *m_am = AssetManager::getInstance();
    Status *m_cur_status = nullptr;
    bool status_switched = false;
    UserCursor m_cursor;
    Bank m_bank{};

    STD() {
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
        m_window->setMouseCursorVisible(false);
        auto mB = std::make_shared<MessageBar>(5, 36, sf::seconds(0.05), sf::Vector2i(0, 400));
        auto nkB = std::make_shared<NumKeyBoard>(10, 3,
                                                 sf::Vector2i(1275, 80), sf::Vector2i(5, 20), sf::Vector2i(100, 50),
                                                 sf::Vector2f(1, 1),
                                                 b1_0, b1_1, b1_2,
                                                 "media/audio/p2_1.wav", "media/audio/p2_2.wav", "media/audio/p1_1.wav",
                                                 "media/audio/p1_2.wav");
        auto lcd = std::make_shared<LCDRowDisplay>(sf::Sprite(), Animation(), sf::Text(), std::string(16, '8'), 16,
                                                   sf::seconds(0.03), "media/jm.otf");
        lcd->standard_user_settings_LCDDisplay(lcd->m_text);
        lcd->scale({1, 2});
        lcd->m_text.setLetterSpacing(1.5);
        lcd->m_text.setPosition(1275, 15);
        lcd->m_bck_sprite.setPosition(1275, 15);
        auto change_button = std::make_shared<Button>(sf::Sprite(), sf::IntRect(1200, 100, 50, 100),
                                                      Animation("hover"), b2_1, b2_2,
                                                      "", "", "media/audio/p2_1.wav", "media/audio/p2_2.wav");
        change_button->m_sprite.setPosition(1200, 100);
        change_button->m_press_f = [this](){this->switchStatus("Ожидание оплаты (купюры)");};
        auto coin_tray = std::make_shared<sf::RectangleShape>(sf::Vector2f(50, 50));
        coin_tray->setPosition(1214, 220);
        auto coin_receiver = std::make_shared<sf::RectangleShape>(sf::Vector2f(50, 50));
        coin_receiver->setPosition(1214, 15);
        auto money_receiver = std::make_shared<sf::RectangleShape>(sf::Vector2f(130, 75));
        money_receiver->setPosition(1370, 365);
        auto coin_dialog = std::make_shared<ChooseDialog>(std::vector<FuncWithDesc>{{"1p.",   []() {}},
                                                                                    {"2p.",   []() {}},
                                                                                    {"5p.",   []() {}},
                                                                                    {"10p.",  []() {}},
                                                                                    {"Close", []() {}}},
                                                          "Insert coin", sf::Vector2f(0, 0), sf::Vector2i(400, 40),
                                                          sf::Vector2f(1, 1));
        auto money_dialog = std::make_shared<ChooseDialog>(std::vector<FuncWithDesc>{{"50p.",   []() {}},
                                                                                     {"100p.",  []() {}},
                                                                                     {"200p.",  []() {}},
                                                                                     {"500p.",  []() {}},
                                                                                     {"1000p.", []() {}},
                                                                                     {"2000p.", []() {}},
                                                                                     {"5000p.", []() {}},
                                                                                     {"Close",  [this]() { switchStatus("Ожидание UI");}}},
                                                           "Insert money", sf::Vector2f(0, 0), sf::Vector2i(400, 40),
                                                           sf::Vector2f(1, 1));

////////////////////////////////////////////////////
        auto rect = std::make_shared<sf::RectangleShape>(sf::Vector2f(1200, 550));
        rect->setFillColor(sf::Color(0, 0, 0, 0));
        rect->setOutlineColor(sf::Color(255, 255, 255));
        rect->setOutlineThickness(2);
        auto rect1 = std::make_shared<sf::RectangleShape>(sf::Vector2f(400, 550));
        rect1->setFillColor(sf::Color(0, 0, 0, 0));
        rect1->setOutlineColor(sf::Color(255, 255, 255));
        rect1->setOutlineThickness(2);
        auto rect2 = std::make_shared<sf::RectangleShape>(sf::Vector2f(800, 550));
        rect2->setFillColor(sf::Color(0, 0, 0, 0));
        rect2->setOutlineColor(sf::Color(255, 255, 255));
        rect2->setOutlineThickness(2);
        auto rect3 = std::make_shared<sf::RectangleShape>(sf::Vector2f(400, 400));
        rect3->setFillColor(sf::Color(0, 0, 0, 0));
        rect3->setOutlineColor(sf::Color(255, 255, 255));
        rect3->setOutlineThickness(2);
////////////////////////////////////////////////////
        mB->leave_message(std::string(40, '8'));
        mB->leave_message(std::string(40, '8'));
        mB->leave_message(std::string(40, '8'));
        mB->leave_message(std::string(40, '8'));
        mB->leave_message(std::string(40, '8'));
        mB->leave_message(std::string(40, '0'));

        m_status["Ожидание UI"] = {{nkB,  change_button},
                                   {nkB,  change_button},
                                   {nkB,  change_button},
                                   {mB,   lcd,   nkB,   change_button},
                                   {rect, rect3, rect2, rect1, mB, lcd, nkB, change_button, coin_tray, coin_receiver, money_receiver}};
        m_status["Ожидание оплаты (монеты)"] = {{coin_dialog},
                                                {coin_dialog},
                                                {coin_dialog},
                                                {mB,   lcd,   nkB,   change_button, coin_dialog},
                                                {rect, rect3, rect2, rect1,         mB, lcd, nkB, change_button, coin_tray, coin_receiver, money_receiver, coin_dialog}};
        m_status["Ожидание оплаты (купюры)"] = {{money_dialog},
                                                {money_dialog},
                                                {money_dialog},
                                                {mB,   lcd,   nkB,   change_button, money_dialog},
                                                {rect, rect3, rect2, rect1,         mB, lcd, nkB, change_button, coin_tray, coin_receiver, money_receiver, money_dialog}};
        m_status["Набор с клавиатуры"] = {{nkB},
                                          {nkB},
                                          {nkB},
                                          {mB,   lcd,   nkB,   change_button},
                                          {rect, rect3, rect2, rect1, mB, lcd, nkB, change_button, coin_tray, coin_receiver, money_receiver}};
        m_status["Проверка ввода"] = {{},
                                      {},
                                      {},
                                      {mB},
                                      {mB}};
        m_status["Забрать товар"] = {{},
                                     {},
                                     {},
                                     {mB},
                                     {mB}};
        switchStatus("Ожидание UI");
    }

    ~STD() {
        m_cur_status = nullptr;
        delete m_am;
    }

    void input() {
        sf::Event m_event{};
        while (m_window->pollEvent(m_event)) {
            switch (m_event.type) {
                case sf::Event::Closed:
                    m_window->close();
                    break;
                case sf::Event::MouseButtonPressed:
                    m_cursor.press(*m_window);
                    break;
                case sf::Event::MouseButtonReleased:
                    m_cursor.release(*m_window);
                    break;
                default:
                    break;
            }
        }
    }

    void switchStatus(const std::string &status_name) {
        m_cur_status = &m_status.at(status_name);
        status_switched = true;
    }

    void update(const sf::Time &dt) {
        if (status_switched) {
            m_cursor.m_hover_objects = m_cur_status->m_hover_objects;
            m_cursor.m_press_objects = m_cur_status->m_press_objects;
            m_cursor.m_release_objects = m_cur_status->m_release_objects;
            status_switched = false;
        }
        m_cursor.hover(*m_window);
        for (auto &i: m_cur_status->m_update_objects)
            i->update(dt);
    }

    void draw() {
        m_window->clear();
        for (auto &i: m_cur_status->m_draw_objects)
            m_window->draw(*i);
        m_cursor.draw(*m_window);
        m_window->display();
    }

    void run() {
        sf::Clock m_clock;
        while (m_window->isOpen()) {
            auto dt = m_clock.restart();
            input();
            update(dt);
            draw();
        }
    }
};

int main() {
    STD std;
    std.run();
    return 0;
}