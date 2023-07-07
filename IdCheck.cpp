#include "CoffeeChecker.h"

std::istream &operator>>(std::istream &is, CoffeeChecker &ic) {
    nlohmann::json j;
    is >> j;
    for (auto &i: j.at("objects")) {
        uint64_t id = i["id"];
        ic.m_content[id] = i;
    }
    return is;
}

bool CoffeeChecker::checkPrice(uint64_t id, uint64_t price) {
    return m_content.contains(id) && (price >= m_content.at(id)["price"]);
}

std::string CoffeeChecker::name(uint64_t id) {
    return m_content.at(id)["name"].get<std::string>();
}

std::string CoffeeChecker::volume(uint64_t id) {
    return m_content.at(id)["volume"].get<std::string>();
}

std::string CoffeeChecker::name_volume(uint64_t id) {
    return name(id) + ", " + volume(id);
}

std::string CoffeeChecker::description(uint64_t id) {
    return m_content.at(id)["description"].get<std::string>();
}

