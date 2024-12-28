#include "../include/Diary.hpp"
#include "../include/error.hpp"
#include "../include/File.hpp"
#include <string>
#include <iostream>
#include <iomanip>

Deal::Deal(bool sign, double money):sign(sign), money(money){}

Diary::Diary() {
    deal_river_.initialise("deal");
}

void Diary::ShowFinance(std::string &Count) {
    int size, count;
    deal_river_.get_info(size, 1);
    if (Count.empty()) {
        count = size;
    } else {
        count = Check::checkBook4(Count);
        if (count > size) {
            error("Invalid\n");
        }
    }
    Deal deal;
    double income = 0, outcome = 0;
    for (int i = 1; i <= count; ++i) {
        deal_river_.read(deal, size - i);
        if (deal.sign) {
            income += deal.money;
        } else {
            outcome += deal.money;
        }
    }
    std::cout << "+ " << std::fixed << std::setprecision(2) << income;
    std::cout << " - " << std::fixed << std::setprecision(2) << outcome << '\n';
}
