#ifndef STD_PRJ_NUMKEYBOARD_H
#define STD_PRJ_NUMKEYBOARD_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include "Button.h"

struct NumKeyBoard : public sf::Drawable {
    sf::Sound m_speaker;
    sf::SoundBuffer m_sbf;
    std::list<Button> m_buttons;
    virtual void keySignal(uint64_t key) {}
    void keySound(uint64_t key, const std::string &filename_prefix);
    static sf::Vector2i getPosition(uint64_t k, uint64_t cols, const sf::Vector2i &distance, const sf::Vector2i &size, const sf::Vector2f &scale);
    NumKeyBoard(uint64_t k, uint64_t cols,
                const sf::Vector2i &distance, const sf::Vector2i &size, const sf::Vector2f &scale,
                Animation &pressAnim, Animation &releaseAnim,
                const sf::SoundBuffer &press_s, const sf::SoundBuffer &release_s,
                const std::string &filename_prefix = "media/audio/c");
    void press(const sf::Vector2i &pos);
    void release(const sf::Vector2i &pos);
    void update(const sf::Time &dt);
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //STD_PRJ_NUMKEYBOARD_H