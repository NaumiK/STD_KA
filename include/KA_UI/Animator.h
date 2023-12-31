#ifndef STD_PRJ_ANIMATOR_H
#define STD_PRJ_ANIMATOR_H

#include <cstdint>
#include <string>
#include <list>
#include <SFML/Graphics.hpp>
#include "Hoverable.h"

struct Animation {
    std::string name, texture_path;
    uint64_t total_frame, columns;
    sf::Vector2i frame_size;
    sf::Time duration_;
    sf::Color mask_color;
    bool isLoop;
    sf::IntRect get_box(uint64_t cur_frame) const;
};

struct AnimationManager {
protected:
    std::map<std::string, Animation> m_Animations;
protected:
    AnimationManager() = default;
    inline static AnimationManager *animationManager_ = nullptr;
public:
    AnimationManager(AnimationManager &) = delete;
    void operator=(const AnimationManager &) = delete;

    static AnimationManager *getInstance();
    void loadFile(const std::string &file_name="media/animations.json");
    static Animation &getAnimation(const std::string &id_name);
};

struct Animator : public Updateable {
    sf::Sprite &m_sprite;
    sf::Time current_time;
    std::list<Animation> m_animations;
    Animation *current_anim = nullptr;
    bool endAnim = true;
    explicit Animator(sf::Sprite &sprite);
    Animation *findAnimation(const std::string &name);
    void switchAnimation(Animation *anim);
    bool switchAnimation(const std::string &name);
    std::string getCurrentAnimationName() const;
    Animation &addAnimation(const Animation &anim);
    void restart();
    void update(const sf::Time &dt) override;
};


#endif //STD_PRJ_ANIMATOR_H
