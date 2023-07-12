#include "Animator.h"
#include "AssetManager.h"
#include <fstream>

int main() {
    AssetManager *am = AssetManager::getInstance();
    auto *aniM = AnimationManager::getInstance();
    aniM->loadFile("media/animations.json");
    auto anim = AnimationManager::getAnimation("b1_2");
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
    delete aniM;
    return 0;
}