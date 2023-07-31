#include "Animator.h"
#include <algorithm>
#include <ranges>
#include "AssetManager.h"
#include "nlohmann/json.hpp"
#include <fstream>

sf::IntRect Animation::get_box(uint64_t cur_frame) const {
    if (total_frame <= cur_frame) throw std::out_of_range("total_frame <= cur_frame");
    return {sf::Vector2i(static_cast<int>(cur_frame % columns) * frame_size.x,
                         static_cast<int>(cur_frame / columns) * frame_size.y), frame_size};
}
///////////////////////////////////////////////////////

AnimationManager *AnimationManager::getInstance() {
    if (animationManager_ == nullptr)
        animationManager_ = new AnimationManager;
    return animationManager_;
}

void AnimationManager::loadFile(const std::string &file_name) {
    nlohmann::json j;
    auto fin = std::ifstream("media/animations.json");
    fin >> j;
    for (auto &i: j.at("objects")) {
        auto id_name = i.at("id_name").get<std::string>();
        auto [w, h] = i.at("frame_size").get<std::array<int, 2>>();
        auto [r, g, b, a] = i.at("mask_color").get<std::array<uint8_t, 4>>();
        m_Animations.emplace(id_name, Animation{i.at("name").get<std::string>(),
                                                i.at("texture_path").get<std::string>(),
                                                i.at("total_frame").get<uint64_t>(),
                                                i.at("columns").get<uint64_t>(),
                                                sf::Vector2i{w, h},
                                                sf::seconds(i.at("duration").get<float>()),
                                                sf::Color(r, g, b, a),
                                                i.at("isLoop").get<bool>()});
    }
}

Animation &AnimationManager::getAnimation(const std::string &id_name) {
    return animationManager_->m_Animations.at(id_name);
}

///////////////////////////////////////////////////////
Animator::Animator(sf::Sprite &sprite)
    : m_sprite(sprite) {}


Animation *Animator::findAnimation(const std::string &name) {
    auto it = std::ranges::find_if(m_animations, [&name](const Animation &anim){return anim.name == name;});
    return (it != m_animations.end() ? &(*it): nullptr);
}

void Animator::switchAnimation(Animation *anim) {
    if (anim != nullptr && !anim->texture_path.empty()) {
        m_sprite.setTexture(AssetManager::getTexture(anim->texture_path, anim->mask_color));
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

Animation &Animator::addAnimation(const Animation &anim) {
    m_animations.emplace_back(anim);
    if (current_anim == nullptr) switchAnimation(&m_animations.back());
    return m_animations.back();
}

void Animator::restart() {
    if (current_anim == nullptr || current_anim->texture_path.empty()) return;
    current_time = sf::Time::Zero;
    endAnim = false;
}

void Animator::update(const sf::Time &dt) {
    if (current_anim == nullptr || current_anim->texture_path.empty()) return;
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
