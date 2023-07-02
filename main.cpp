#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Animator.h"
#include "NumKeyBoard.h"


int main() {
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
    sf::RenderWindow window(sf::VideoMode(800, 800), "Test button",
                            sf::Style::Titlebar | sf::Style::Close);

    sf::SoundBuffer buffer11, buffer12;
    buffer11.loadFromFile("media/audio/p2_1.wav");
    buffer12.loadFromFile("media/audio/p2_2.wav");
    sf::SoundBuffer buffer1, buffer2;
    buffer1.loadFromFile("media/audio/p1_1.wav");
    buffer2.loadFromFile("media/audio/p1_2.wav");
    NumKeyBoard keyBoard(10, 3, {5, 20}, {100, 50}, {2, 2}, b1_1, b1_2, buffer1, buffer2);

    sf::Clock clock;
    sf::Sound sound;
    sf::SoundBuffer buffer;
    buffer.loadFromFile("media/audio/done.wav");
    sound.setBuffer(buffer);
    while (window.isOpen()) {
        auto dt = clock.restart();
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                keyBoard.press(sf::Mouse::getPosition(window));
                break;
            case sf::Event::MouseButtonReleased:
                keyBoard.release(sf::Mouse::getPosition(window));
                break;
            case sf::Event::KeyPressed:
                sound.play();
                break;
            default:
                break;
            }
        }
        window.clear();
        keyBoard.update(dt);
        window.draw(keyBoard);
        window.display();
    }
    return 0;
}