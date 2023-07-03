#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <coroutine>


void func(sf::Sound &sound, sf::SoundBuffer &buffer) {
    sound.setBuffer(buffer);
    sound.play();
}

struct StrWindow {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;
    explicit StrWindow (handle_type h) : coro(h) {}
    StrWindow (const StrWindow &) = delete;
    StrWindow &operator=(const StrWindow &) = delete;
    StrWindow (StrWindow &&window) : coro(window.coro) {
        window.coro = nullptr;
    }
    StrWindow &operator=(StrWindow &&window) {
        coro = window.coro;
        window.coro = nullptr;
        return *this;
    }
    ~StrWindow() { if (coro) coro.destroy(); }
    sf::String getValue() const {
        coro.resume();
        return coro.promise().current_window;
    }
    bool next() const {
        coro.resume();
        return not coro.done();
    }
    struct promise_type {
        promise_type() = default;
        ~promise_type() = default;
        sf::String current_window;
        StrWindow get_return_object() {
            return StrWindow(handle_type::from_promise(*this));
        }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        std::suspend_always yield_value(const sf::String &window) {
            current_window = window;
            return {};
        }
        void return_void(){}
        void unhandled_exception(){ return std::rethrow_exception(std::current_exception()); }
    };
};

StrWindow test(const std::string &s, int l, int dl = 1) {
    if (l >= s.length()) co_yield sf::String::fromUtf8(s.begin(), s.end());
    else {
        co_yield sf::String::fromUtf8(s.begin(), s.begin() + l);
        for (int i = 0;; i = (i + dl) % (s.length() - l + 1)) {
            std::cout << i << " ";
            co_yield sf::String::fromUtf8(s.begin() + i, s.begin() + i + l);
        }
    }
}

sf::String strWindow(const std::string &str, uint64_t inx, uint64_t l) {
    if (l >= str.length()) return sf::String::fromUtf8(str.begin(), str.end());
    inx = inx % (str.length() - l + 1);
    return sf::String::fromUtf8(str.begin() + (int)inx, str.begin() + (int)(inx + l));
}

int main() {
    sf::Sound sound, sound2;
    sf::SoundBuffer buffer1, buffer2, buffer3;
    buffer1.loadFromFile("media/audio/p1_1.wav");
    buffer2.loadFromFile("media/audio/p1_2.wav");
    std::array<std::string, 10> c = {};
    for (size_t i = 0; i < 10; ++i) c[i] = "media/audio/c" + std::to_string(i) + ".wav";
    size_t cnt = 0;
    buffer3.loadFromFile(c[(cnt++) % 10]);
    sound.setBuffer(buffer1);
    sf::Font font;
    sf::RenderWindow window(sf::VideoMode(800, 200), "Test font", sf::Style::Titlebar | sf::Style::Close);
    // sf::RenderWindow window(sf::VideoMode(800, 200), "Test font", sf::Style::FullScreen);
    window.setMouseCursorVisible(false);
    sf::Texture cursorT;
    if (!cursorT.loadFromFile("media/images/cursor/Point.png")) return 1;
    sf::Sprite cursor;
    cursor.setTexture(cursorT);
    // cursor.setScale({2, 2});
    sf::Text text;
    text.setFillColor(sf::Color(60, 50, 255));
    text.setCharacterSize(30);
    text.setPosition(10, 10);
    std::string message = "Insert money and choose your drink :)";
//    std::string_view sb = message;
    text.setString(sf::String::fromUtf8(message.begin(), message.begin() + 6));
//    if (!(font.loadFromFile("media/EpilepsySans.ttf"))) {
    if (!(font.loadFromFile("media/DisplayOTF.otf"))) {
//    if (!(font.loadFromFile("media/pixel_lcd_7.ttf"))) {
        std::cout << "T_T" << std::endl;
        return 1;
    }
    text.setFont(font);
    text.setLetterSpacing(2);
    auto gen = test(message, 30, 3);
    uint64_t inx = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    text.setFillColor(sf::Color::Yellow);
                    func(sound, buffer1);
                    break;
                case sf::Event::MouseButtonReleased:
                    text.setFillColor(sf::Color::Green);
                    text.setString(gen.getValue());
//                    text.setString(strWindow(message, inx, 30));
//                    inx += 2;
                    buffer3.loadFromFile(c[(cnt++) % 10]);
                    func(sound, buffer2);
                    func(sound2, buffer3);
                    break;
                default:
                    break;
            }
        }
        window.clear();
        window.draw(text);
        auto pos = sf::Mouse::getPosition(window);
        cursor.setPosition(pos.x - 16, pos.y - 12);
        window.draw(cursor);
        window.display();
    }
    return 0;
}