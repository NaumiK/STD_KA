#include <utility>

#include "KA_UI.h"

Animation test = {"press", "media/images/b1_1.png",
                  17, 5,
                  {100, 50}, sf::seconds(0.8),
                  {0, 255, 0}, false};

struct MessageBar : sf::Drawable {
private:
    std::vector<std::pair<sf::Text, std::string>> m_lines;
    uint64_t m_message_cnt;
    uint64_t m_length;
    sf::Time m_delta_duration, m_current_time;
    sf::Vector2i m_pos;
    std::string m_font;
    int m_space;
public:
    MessageBar(uint64_t messageCnt, uint64_t length, const sf::Time &dt, const sf::Vector2i &pos, int space = 30,
               std::string font = "media/basis33.ttf")
            : m_message_cnt(messageCnt), m_length(length), m_delta_duration(dt), m_pos(pos), m_font(std::move(font)),
              m_space(space) {
        for (uint64_t i = 0; i < m_message_cnt; ++i) {
            m_lines.emplace_back();
            m_lines.back().first.setPosition({(float) m_pos.x, (float) (m_pos.y + m_space * i)});
            standard_text(m_lines.back().first);
        }
    }

    void update(sf::Time dt) {
        m_current_time += dt;
        for (size_t i = 0; i < m_message_cnt; ++i) {
            auto &[m_text, m_string] = m_lines[i];
            float scaledTime = (m_current_time.asSeconds() / (m_delta_duration.asSeconds() *
                                                              static_cast<float>(m_string.length())));
            auto cur_frame = static_cast<uint64_t>(scaledTime * static_cast<float>(m_length));
            m_text.setString(strRowWindow(m_string, cur_frame, m_length));
        }
    }

    void leave_message(const std::string &message) {
        for (uint64_t i = 0; i < m_message_cnt - 1; ++i) {
            m_lines[i].second = m_lines[i + 1].second;
        }
        m_lines[m_message_cnt - 1].second = (message.length() < m_length ? message : std::string(m_length, ' ') +
                                                                                     message +
                                                                                     std::string(m_length, ' '));
    }

private:
    void standard_text(sf::Text &text) const {
        text.setFont(AssetManager::getFont(m_font));
        text.setCharacterSize(25);
    }

    static sf::String strRowWindow(const std::string &str, uint64_t frame, uint64_t l) {
        if (l >= str.length()) return sf::String::fromUtf8(str.begin(), str.end());
        frame = frame % (str.length() - l + 1);
        return sf::String::fromUtf8(str.begin() + (int) frame, str.begin() + (int) (frame + l));
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        for (auto &i: m_lines)
            target.draw(i.first, states);
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