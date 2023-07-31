#ifndef STD_PRJ_FUNCWITHDESC_H
#define STD_PRJ_FUNCWITHDESC_H

#include <functional>

struct FuncWithDesc {
    std::string desc;
    std::function<void()> func;
};

#endif //STD_PRJ_FUNCWITHDESC_H
