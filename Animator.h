#ifndef STD_PRJ_ANIMATOR_H
#define STD_PRJ_ANIMATOR_H

#include <cstdint>
#include <string>
#include <list>
#include <SFML/Graphics.hpp>

struct Animation {
    std::string name, texture_path;
    uint64_t total_frame, columns;
    sf::Vector2i frame_size;
    sf::Time duration_;
    sf::Color mask_color;
    bool isLoop;
    sf::IntRect get_box(uint64_t cur_frame) const;
    sf::Image get_image() const;
    sf::Texture get_texture(uint64_t cur_frame) const;
};

struct Animator {
    sf::Sprite &m_sprite;
    sf::Time current_time;
    std::list<Animation> m_animations;
    Animation *current_anim = nullptr;
    sf::Texture m_tex;
    bool endAnim = true;
    explicit Animator(sf::Sprite &sprite);
    Animation *findAnimation(const std::string &name);
    void switchAnimation(Animation *anim);
    bool switchAnimation(const std::string &name);
    std::string getCurrentAnimationName() const;
    Animation &addAnimation(Animation &&anim);
    Animation &addAnimation(const Animation &anim);
    void restart();
    void update(const sf::Time &dt);
};


#endif //STD_PRJ_ANIMATOR_H
