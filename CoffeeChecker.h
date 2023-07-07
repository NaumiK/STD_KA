#ifndef STD_PRJ_COFFEECHECKER_H
#define STD_PRJ_COFFEECHECKER_H

#include <istream>
#include <map>
#include <nlohmann/json.hpp>

struct CoffeeChecker {
    std::map<uint64_t, nlohmann::json> m_content;
public:
    CoffeeChecker() = default;

    friend std::istream &operator>>(std::istream &is, CoffeeChecker &ic);
    bool checkPrice(uint64_t id, uint64_t price);
    std::string name(uint64_t id);
    std::string volume(uint64_t id);
    std::string name_volume(uint64_t id);
    std::string description(uint64_t id);
};

#endif //STD_PRJ_COFFEECHECKER_H
