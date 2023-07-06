#ifndef STD_PRJ_BANK_H
#define STD_PRJ_BANK_H

#include <array>

struct Bank {
private:
    std::array<uint64_t, 4> coins_available{0};
    uint64_t curr_acc{0};
public:
    void add_coin(uint64_t value);
    void add_money(uint64_t value);
    uint64_t get_curr_acc() const;
    std::array<uint64_t, 4> get_change();
};

#endif //STD_PRJ_BANK_H
