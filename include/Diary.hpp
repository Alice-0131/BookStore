#ifndef DIARY_HPP
#define DIARY_HPP

#include <string>
#include "File.hpp"
#include "Book.hpp"
#include "Account.hpp"

struct Deal {
    bool sign = false;
    double money = 0;
    char book[21] = {0};

    Deal() = default;
    Deal(bool sign, double money, char book[]);
};

struct Operation {
    char UserID[31] = {0};
    char type[10] = {0};
    char Object[31] = {0};
    char Order[31] = {0};
    char Content[75] = {0};

    Operation(char UserID[], std::string type, char Object[], std::string Order, std::string& Content);
    Operation() = default;
};

class BookSystem;
class LoginStack;

class Diary {
    friend class BookSystem;
    friend class LoginStack;

    MemoryRiver<Deal> deal_river_;
    MemoryRiver<Operation> operation_river_;
public:
    Diary();
    void ShowFinance(std::string& Count);
    void ReportFinance(BookSystem& book_system);
    void ReportEmployee();
    void Log(BookSystem& book_system);
};

#endif //DIARY_HPP
