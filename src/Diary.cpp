#include "../include/Diary.hpp"
#include "../include/error.hpp"
#include "../include/File.hpp"
#include "../include/Book.hpp"
#include "../include/Account.hpp"
#include <string>
#include <iostream>
#include <iomanip>

Deal::Deal(bool sign, double money, char book[]):sign(sign), money(money) {
    strcpy(this->book, book);
}

Operation::Operation(char UserID[], std::string type, char Object[], std::string Order, std::string& Content) {
    strcpy(this->UserID, UserID);
    strcpy(this->type, type.c_str());
    strcpy(this->Object, Object);
    strcpy(this->Order, Order.c_str());
    strcpy(this->Content, Content.c_str());
}

Diary::Diary() {
    deal_river_.initialise("deal");
    operation_river_.initialise("operation");
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

void Diary::ReportFinance(BookSystem& book_system) {
    int size;
    book_system.book_river_.get_info(size, 1);
    std::cout << "We have " << size;
    if (size <= 1) {
        std::cout << " kind of book in the bookstore.\n";
    } else {
        std::cout << " kinds of books in the bookstore.\n";
    }
    deal_river_.get_info(size, 1);
    std::cout << "Total Transactions: " << size << "\n";
    Deal deal;
    double income = 0, outcome = 0;
    int num_of_income = 0, num_of_outcome = 0;
    for (int i = 0; i < size; ++i) {
        deal_river_.read(deal, i);
        if (deal.sign) {
            income += deal.money;
            ++num_of_income;
        } else {
            outcome += deal.money;
            ++num_of_outcome;
        }
    }
    std::cout << "There is " << num_of_outcome << " expense and " << num_of_income << " income.\n";
    std::cout << "Total expenditure is " << std::fixed << std::setprecision(2) << outcome << ".\n";
    std::cout << "Total income is " << std::fixed << std::setprecision(2) << income << ".\n";
}

void Diary::ReportEmployee() {
    int size;
    operation_river_.get_info(size, 1);
    if (size <= 1) {
        std::cout << "There is " << size << " operation in total.\n";
    } else {
        std::cout << "There are " << size << " operations in total.\n";
    }
    std::cout << "UserID\t" << "type\t" << "Object\t" << "Order\t" << "Content\n";
    Operation operation;
    for (int i = 0; i < size; ++i) {
        operation_river_.read(operation, i);
        std::cout << operation.UserID << '\t' << operation.type << '\t' << operation.Object << '\t';
        std::cout << operation.Order << '\t' << operation.Content << '\n';
    }
}

void Diary::Log(BookSystem& book_system) {
    this->ReportEmployee();
    int size;
    deal_river_.get_info(size, 1);
    if (size <= 1) {
        std::cout << "There is " << size << " transaction in total.\n";
    } else {
        std::cout << "There are " << size << " transactions in total.\n";
    }
    std::cout << "The Book's ISBN\t" << "Income and Expenditure\n";
    Deal deal;
    for (int i = 0; i < size; ++i) {
        deal_river_.read(deal, i);
        std::cout << deal.book << '\t';
        if (deal.sign) {
            std::cout << "Income: ";
        } else {
            std::cout << "Expenditure: ";
        }
        std::cout << deal.money << '\n';
    }
}
