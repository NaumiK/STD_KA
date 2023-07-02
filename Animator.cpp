#include "Animator.h"
#include <algorithm>
#include <ranges>

sf::IntRect Animation::get_box(uint64_t cur_frame) const {
    if (total_frame <= cur_frame) throw std::out_of_range("total_frame <= cur_frame");
    return {sf::Vector2i(static_cast<int>(cur_frame % columns) * frame_size.x,
                         static_cast<int>(cur_frame / columns) * frame_size.y), frame_size};
}

sf::Image Animation::get_image() const {
    sf::Image res;
    res.loadFromFile(texture_path);
    res.createMaskFromColor(mask_color);
    return res;
}

sf::Texture Animation::get_texture(uint64_t cur_frame) const {
    sf::Texture res;
    res.loadFromImage(get_image(), get_box(cur_frame));
    return res;
}
///////////////////////////////////////////////////////
Animator::Animator(sf::Sprite &sprite)
    : m_sprite(sprite) {}


Animation *Animator::findAnimation(const std::string &name) {
    auto it = std::ranges::find_if(m_animations, [&name](const Animation &anim){return anim.name == name;});
    return (it != m_animations.end() ? &(*it): nullptr);
}

void Animator::switchAnimation(Animation *anim) {
    if (anim != nullptr) {
        m_tex.loadFromImage(anim->get_image());
        m_sprite.setTexture(m_tex);
    }
    current_anim = anim;
    current_time = sf::Time::Zero;
}

bool Animator::switchAnimation(const std::string &name) {
    auto anim = findAnimation(name);
    if (anim != nullptr) {
        switchAnimation(anim);
        return true;
    }
    return false;
}

std::string Animator::getCurrentAnimationName() const {
    if (current_anim != nullptr) return current_anim->name;
    return "";
}

Animation &Animator::addAnimation(Animation &&anim) {
    m_animations.emplace_back(anim);
    if (current_anim == nullptr) switchAnimation(&m_animations.back());
    return m_animations.back();
}

Animation &Animator::addAnimation(const Animation &anim) {
    m_animations.emplace_back(anim);
    if (current_anim == nullptr) switchAnimation(&m_animations.back());
    return m_animations.back();
}

void Animator::restart() {
    current_time = sf::Time::Zero;
    endAnim = false;
}

void Animator::update(const sf::Time &dt) {
    if (current_anim == nullptr) return;
    if (!endAnim) current_time += dt;
    float scaledTime = (current_time.asSeconds() / current_anim->duration_.asSeconds());
    uint64_t cur_frame = scaledTime * current_anim->total_frame;
    if (current_anim->isLoop) cur_frame %= current_anim->total_frame;
    else if (cur_frame >= current_anim->total_frame) {
        cur_frame = current_anim->total_frame - 1;
        endAnim = true;
    }
    m_sprite.setTextureRect(current_anim->get_box(cur_frame));
}
