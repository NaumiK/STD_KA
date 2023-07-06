#include "Animator.h"
#include "AssetManager.h"

int main() {
    AssetManager *am = AssetManager::getInstance();
    Animation anim = {"press", "media/images/b1_1.png",
                      17, 5,
                      {100, 50}, sf::seconds(0.8),
                      {0, 255, 0}, false};
    sf::Sprite sprite;
    Animator ar(sprite);
    ar.addAnimation(anim);
    std::unique_ptr<sf::RenderWindow> window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1600, 550),
                                                                                    "Test Manager",
                                                                                    sf::Style::Titlebar |
                                                                                    sf::Style::Close);
    sf::Clock clock;
    while (window->isOpen()) {
        auto dt = clock.restart();
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed)
                ar.restart();
            if (event.type == sf::Event::Closed)
                window->close();
        }
        ar.update(dt);
        window->clear();
        window->draw(sprite);
        window->display();
    }
    delete am;
    return 0;
}