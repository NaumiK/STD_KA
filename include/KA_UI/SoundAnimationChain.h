#ifndef STD_PRJ_SOUNDANIMATIONCHAIN_H
#define STD_PRJ_SOUNDANIMATIONCHAIN_H

#include "Animator.h"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "AssetManager.h"
#include <array>
#include <functional>
#include <utility>
#include <iostream>

using stsst = std::tuple<std::string, std::string, sf::Time>;

template<const uint64_t N>
struct SoundAnimationChain : public Updateable, sf::Drawable {
    sf::Sprite m_sprite;
    Animator m_ar;
    sf::Sound m_sound;
    std::array<std::tuple<std::string, std::string, sf::Time>, N> m_chain;
    sf::Time m_current_time = sf::Time::Zero;
    uint64_t m_current_animation = 0;

    SoundAnimationChain(sf::Sprite sprite, std::array<std::tuple<std::string, std::string, sf::Time>, N> chain,
                        std::function<void()> atTheEnd = []() {});

    void update(const sf::Time &dt) override;

    void step_passed();

    void restart();

    std::function<void()> m_atTheEnd;
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

template<const uint64_t N>
void SoundAnimationChain<N>::step_passed() {
    auto &[sound_name, anim_name, timer] = m_chain[m_current_animation % N];
    m_current_time = sf::Time::Zero;
    m_ar.switchAnimation(anim_name);
    m_ar.restart();
    m_sound.setBuffer(AssetManager::getSoundBuffer(sound_name));
    m_sound.play();
}

template<const uint64_t N>
void SoundAnimationChain<N>::restart() {
    m_current_time = sf::Time::Zero;
    m_current_animation = 0;
    step_passed();
}

template<const uint64_t N>
void SoundAnimationChain<N>::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(m_sprite, states);
}

template<const uint64_t N>
SoundAnimationChain<N>::SoundAnimationChain(sf::Sprite sprite,
                                            std::array<std::tuple<std::string, std::string, sf::Time>, N> chain,
                                            std::function<void()> atTheEnd)
        : m_chain(chain), m_sprite(std::move(sprite)), m_ar(m_sprite), m_atTheEnd(std::move(atTheEnd)) {
}

template<const uint64_t N>
void SoundAnimationChain<N>::update(const sf::Time &dt) {
    m_ar.update(dt);
    m_current_time += dt;
    auto &[sound_name, anim_name, timer] = m_chain[m_current_animation % N];
    if (m_current_time >= timer) {
        m_current_animation++;
        if (m_current_animation >= N) {
            m_atTheEnd();
            return;
        }
        step_passed();
    }
}


#endif //STD_PRJ_SOUNDANIMATIONCHAIN_H
