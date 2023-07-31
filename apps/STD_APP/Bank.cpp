#include "Bank.h"

void Bank::add_coin(uint64_t value) {
    switch (value) {
        case 10: coins_available[0]++; break;
        case 5:  coins_available[1]++; break;
        case 2:  coins_available[2]++; break;
        case 1:  coins_available[3]++; break;
        default:                       break;
    }
}

void Bank::add_money(uint64_t value) {
    add_coin(value);
    curr_acc += value;
}

uint64_t Bank::get_curr_acc() const {
    return curr_acc;
}

std::array<uint64_t, 4> Bank::get_change() {
    decltype(get_change()) res{0}, nom = {10, 5, 2, 1};
    for (size_t i = 0; i < res.size(); ++i) {
        res[i] = curr_acc / nom[i];
        if (res[i] <= coins_available[i]) coins_available[i] -= res[i];
        else res[i] = coins_available[i], coins_available[i] = 0;
        curr_acc -= nom[i] * res[i];
    }
    curr_acc = 0;
    return res;
}

void Bank::spend_f_curr_acc(uint64_t value) {
    curr_acc -= value;
}
