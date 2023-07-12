//
// Created by danii on 12.07.2023.
//

#ifndef STD_PRJ_ANIMATIONMANAGER_H
#define STD_PRJ_ANIMATIONMANAGER_H

#include "Animator.h"
#include <nlohmann/json.hpp>
#include <fstream>

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


#endif //STD_PRJ_ANIMATIONMANAGER_H
