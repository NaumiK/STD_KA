#ifndef STD_PRJ_NUMKEYBOARD_H
#define STD_PRJ_NUMKEYBOARD_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include "Button.h"
#include "Hoverable.h"

struct NumKeyBoard : public sf::Drawable, public Hoverable {
    sf::Sound m_speaker;
    std::list<Button> m_buttons;
    virtual void keySignal(uint64_t key) {}
    void keySound(uint64_t key, const std::string &filename_prefix);
    static sf::Vector2i getPosition(uint64_t k, uint64_t cols, const sf::Vector2i &distance, const sf::Vector2i &size, const sf::Vector2f &scale);
    NumKeyBoard(uint64_t k, uint64_t cols,
                const sf::Vector2i &pos0, const sf::Vector2i &distance, const sf::Vector2i &size, const sf::Vector2f &scale,
                Animation &pressAnim, Animation &releaseAnim,
                const std::string &press_s, const std::string &release_s,
                const std::string &filename_prefix = "media/audio/c");
    void press(const sf::Vector2i &pos);
    void release(const sf::Vector2i &pos);
    void update(const sf::Time &dt);
    bool contains(const sf::Vector2i &vr) const override;
    void hover(const sf::Vector2i &vr) override;
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //STD_PRJ_NUMKEYBOARD_H
