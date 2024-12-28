#ifndef DIARY_HPP
#define DIARY_HPP

#include <string>
#include "File.hpp"

struct Deal {
    bool sign = false;
    double money = 0;

    Deal() = default;
    Deal(bool sign, double money);
};

class Diary {
    friend class BookSystem;
    MemoryRiver<Deal> deal_river_;
public:
    Diary();
    void ShowFinance(std::string& Count);
};

#endif //DIARY_HPP
