#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <array>


void func(sf::Sound &sound, sf::SoundBuffer &buffer) {
    sound.setBuffer(buffer);
    sound.play();
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
    text.setFillColor(sf::Color::Green);
    text.setCharacterSize(80);
    std::string message = "¤∫₽$£¥\nВведите сумму:";
    text.setString(sf::String::fromUtf8(message.begin(), message.end()));
    if (!(font.loadFromFile("media/EpilepsySans.ttf"))) {
        std::cout << "T_T" << std::endl;
        return 1;
    }
    text.setFont(font);
    
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