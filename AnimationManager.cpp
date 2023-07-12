#include "AnimationManager.h"

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