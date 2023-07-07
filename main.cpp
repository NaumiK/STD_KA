#include <utility>
#include <sstream>
#include <fstream>

#include "KA_UI.h"
#include "Bank.h"
#include "Status.h"
#include "CoffeeChecker.h"
#include "SoundAnimationChain.h"

struct STD {
    std::unique_ptr<sf::RenderWindow> m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1600, 550),
                                                                                    "Test Manager",
                                                                                    sf::Style::Close);
    std::map<std::string, Status> m_status;
    const std::string hello_message = "Insert money and choose your drink :)";
    AssetManager *m_am = AssetManager::getInstance();
    Status *m_cur_status = nullptr;
    bool status_switched = false;
    UserCursor m_cursor;
    Bank m_bank{};
    std::shared_ptr<MessageBar> m_mB = std::make_shared<MessageBar>(5, 36, sf::seconds(0.05), sf::Vector2i(0, 400));
    std::shared_ptr<LCDRowDisplay> m_lcd = std::make_shared<LCDRowDisplay>(sf::Sprite(), Animation(), sf::Text(),
                                                                           hello_message, 16,
                                                                           sf::seconds(0.03), "media/jm.otf");

    void add_money(uint64_t x) {
        m_bank.add_money(x);
        m_mB->leave_message("You have deposited " + std::to_string(x) + "p. to your account, your current account is " +
                            std::to_string(m_bank.get_curr_acc()) + "p.");
        m_lcd->set_string(std::to_string(m_bank.get_curr_acc()) + "p.");
    }

    void get_change() {
        auto [p10, p5, p2, p1] = m_bank.get_change();
        std::stringstream ss;
        ss << "You have received the change: 10p.(" << p10 << "), 5p.(" << p5 << "), 2p.(" << p2 << "), 1p.(" << p1
           << ")";
        m_mB->leave_message(ss.str());
        m_lcd->set_string(hello_message);
    }

    CoffeeChecker m_ic;
    uint64_t m_id = 0;

    struct IdInput : public NumKeyBoard {
        STD *m_std = nullptr;
        uint64_t m_max_digit = 1000, m_digit = m_max_digit;

        static std::string printId(uint64_t id, uint64_t digit) {
            std::stringstream ss;
            for (; digit != 0; digit /= 10) ss << (id / digit) % 10;
            return ss.str();
        }

        void keySignal(uint64_t key) override {
            if (m_std != nullptr && m_digit == m_max_digit)
                m_std->switchStatus("Набор с клавиатуры");
            if (m_digit > 0) {
                m_std->m_id += m_digit * key;
                m_digit /= 10;
            }
            if (m_digit == 0) {
                if (m_std->m_ic.m_content.contains(m_std->m_id))
                    m_std->m_mB->leave_message("You have chosen " + m_std->m_ic.name_volume(m_std->m_id));
                if (m_std->m_ic.checkPrice(m_std->m_id, m_std->m_bank.get_curr_acc())) {
                    m_std->m_drink_is_cooking->restart();
                    m_std->switchStatus("Приготовление напитка");
                    return;
                } else {
                    m_std->m_lcd->set_string("Not enough money or wrong id");
                    m_std->switchStatus("Ожидание UI");
                    reset();
                    return;
                }
            }
            m_std->m_lcd->set_string(printId(m_std->m_id, m_max_digit));
        }

        void reset() {
            m_std->m_id = 0;
            m_digit = m_max_digit;
        }

        IdInput(uint64_t k, uint64_t cols,
                const sf::Vector2i &pos0, const sf::Vector2i &distance, const sf::Vector2i &size,
                const sf::Vector2f &scale,
                const Animation &hoverAnim, const Animation &pressAnim, const Animation &releaseAnim,
                const std::string &hover_s, const std::string &unhover_s,
                const std::string &press_s, const std::string &release_s,
                const std::string &filename_prefix = "media/audio/c")
                : NumKeyBoard(k, cols, pos0, distance, size, scale, hoverAnim, pressAnim, releaseAnim,
                              hover_s, unhover_s, press_s, release_s, filename_prefix) {}
    };

    std::shared_ptr<SoundAnimationChain<2>> m_drink_is_cooking = std::make_shared<SoundAnimationChain<2>>(sf::Sprite(),
                                                                                                          std::array<stsst, 2>{
                                                                                                                  stsst{"media/audio/click_to_start.ogg",
                                                                                                                        "hover",
                                                                                                                        sf::seconds(
                                                                                                                                2)},
                                                                                                                  stsst{"media/audio/addingredient.wav",
                                                                                                                        "press",
                                                                                                                        sf::seconds(
                                                                                                                                3)}},
                                                                                                          [this]() {
                                                                                                              switchStatus(
                                                                                                                      "Забрать товар");
                                                                                                          });

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
        auto nkB = std::make_shared<IdInput>(10, 3,
                                             sf::Vector2i(1275, 100), sf::Vector2i(5, 20), sf::Vector2i(100, 50),
                                             sf::Vector2f(1, 1),
                                             b1_0, b1_1, b1_2,
                                             "media/audio/p2_1.wav", "media/audio/p2_2.wav", "media/audio/p1_1.wav",
                                             "media/audio/p1_2.wav");
        nkB->m_std = this;
        std::ifstream fin("media/coffee.json");
        fin >> m_ic;
        m_lcd->standard_user_settings_LCDDisplay(m_lcd->m_text);
        m_lcd->scale({1, 2});
        m_lcd->m_text.setLetterSpacing(1.5);
        m_lcd->m_text.setPosition(1275, 15);
        m_lcd->m_bck_sprite.setPosition(1275, 15);
        auto change_button = std::make_shared<Button>(sf::Sprite(), sf::IntRect(1200, 100, 50, 100),
                                                      Animation("hover"), b2_1, b2_2,
                                                      "", "", "media/audio/p2_1.wav", "media/audio/p2_2.wav");
        change_button->m_press_f = [this]() { get_change(); };
        auto coin_tray = std::make_shared<sf::RectangleShape>(sf::Vector2f(50, 50));
        coin_tray->setPosition(1214, 220);
//        auto coin_receiver = std::make_shared<sf::RectangleShape>(sf::Vector2f(50, 50));
//        coin_receiver->setPosition(1214, 15);
        auto coin_receiver = std::make_shared<Button>(sf::Sprite(), sf::IntRect(1214, 15, 100, 50), b1_0, b1_1, b1_2,
                                                      "", "", "", "");
        coin_receiver->scale({0.5, 1.0});
        coin_receiver->m_press_f = [this]() { this->switchStatus("Ожидание оплаты (монеты)"); };
//        auto money_receiver = std::make_shared<sf::RectangleShape>(sf::Vector2f(130, 75));
//        money_receiver->setPosition(1370, 365);
        auto money_receiver = std::make_shared<Button>(sf::Sprite(), sf::IntRect(1370, 365, 100, 50), b1_0, b1_1, b1_2);
        money_receiver->scale({1.3, 1.5});
        money_receiver->m_press_f = [this]() { this->switchStatus("Ожидание оплаты (купюры)"); };
        auto coin_dialog = std::make_shared<ChooseDialog>(std::vector<FuncWithDesc>{
                                                                  {"1p.",   [this]() { add_money(1); }},
                                                                  {"2p.",   [this]() { add_money(2); }},
                                                                  {"5p.",   [this]() { add_money(5); }},
                                                                  {"10p.",  [this]() { add_money(10); }},
                                                                  {"Close", [this]() { switchStatus("Ожидание UI"); }}},
                                                          "Insert coin", sf::Vector2f(0, 0), sf::Vector2i(400, 40),
                                                          sf::Vector2f(1, 1));
        auto money_dialog = std::make_shared<ChooseDialog>(std::vector<FuncWithDesc>{
                                                                   {"50p.",   [this]() { add_money(50); }},
                                                                   {"100p.",  [this]() { add_money(100); }},
                                                                   {"200p.",  [this]() { add_money(200); }},
                                                                   {"500p.",  [this]() { add_money(500); }},
                                                                   {"1000p.", [this]() { add_money(1000); }},
                                                                   {"2000p.", [this]() { add_money(2000); }},
                                                                   {"5000p.", [this]() { add_money(5000); }},
                                                                   {"Close",  [this]() { switchStatus("Ожидание UI"); }}},
                                                           "Insert money", sf::Vector2f(0, 0), sf::Vector2i(400, 40),
                                                           sf::Vector2f(1, 1));
//        auto m_drink_is_cooking = std::make_shared<Button>(sf::Sprite(), sf::IntRect(), b1_0, b1_1, b1_2, "");
        m_drink_is_cooking->m_sprite.setPosition(800, 0);
        m_drink_is_cooking->m_ar.addAnimation(b1_0);
        m_drink_is_cooking->m_ar.addAnimation(b1_1);
        auto drink_is_cooked = std::make_shared<Button>(sf::Sprite(), sf::IntRect(800, 0, 100, 50), b1_0, b1_1, b1_2,
                                                        "");
        drink_is_cooked->m_sprite.setPosition(800, 0);
        drink_is_cooked->scale({4, 11});
        drink_is_cooked->m_press_f = [this]() {
            m_bank.spend_f_curr_acc(m_ic.price(m_id));
            m_lcd->set_string(std::to_string(m_bank.get_curr_acc()) + "p.");
            switchStatus("Ожидание UI");
        };
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
        auto rect4 = std::make_shared<sf::RectangleShape>(sf::Vector2f(312, 80));
        rect4->setFillColor(sf::Color(66, 138, 245));
        rect4->setPosition(1275, 0);
////////////////////////////////////////////////////

        m_status["Ожидание UI"] = {{nkB,  change_button, coin_receiver, money_receiver},
                                   {nkB,  change_button, coin_receiver, money_receiver},
                                   {nkB,  change_button, coin_receiver, money_receiver},
                                   {m_mB, m_lcd,         nkB,           change_button, coin_receiver, money_receiver},
                                   {rect, rect3,         rect2,         rect1,         rect4,         m_mB, m_lcd, nkB, change_button, coin_tray, coin_receiver, money_receiver, money_receiver}};
        m_status["Ожидание оплаты (монеты)"] = {{coin_dialog},
                                                {coin_dialog},
                                                {coin_dialog},
                                                {m_mB, m_lcd, coin_dialog, coin_receiver},
                                                {rect, rect3, rect2,       rect1, m_mB, m_lcd, nkB, change_button, coin_tray, coin_receiver, money_receiver, coin_dialog}};
        m_status["Ожидание оплаты (купюры)"] = {{money_dialog},
                                                {money_dialog},
                                                {money_dialog},
                                                {m_mB, m_lcd, money_dialog, money_receiver},
                                                {rect, rect3, rect2,        rect1, m_mB, m_lcd, nkB, change_button, coin_tray, coin_receiver, money_receiver, money_dialog}};
        m_status["Набор с клавиатуры"] = {{nkB},
                                          {nkB},
                                          {nkB},
                                          {m_mB, m_lcd, nkB,   change_button},
                                          {rect, rect3, rect2, rect1, m_mB, m_lcd, nkB, change_button, coin_tray, coin_receiver, money_receiver}};
        m_status["Приготовление напитка"] = {{},
                                             {},
                                             {},
                                             {m_mB, m_drink_is_cooking},
                                             {m_mB, m_drink_is_cooking, rect, rect1, rect2, rect3}};
        m_status["Забрать товар"] = {{drink_is_cooked},
                                     {drink_is_cooked},
                                     {drink_is_cooked},
                                     {m_mB, drink_is_cooked},
                                     {m_mB, drink_is_cooked}};
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