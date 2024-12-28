#ifndef BOOK_HPP
#define BOOK_HPP

#include "Account.hpp"
#include "File.hpp"
#include "Diary.hpp"

struct Book {
    char ISBN[21] = {0};
    char BookName[61] = {0};
    char Author[61] = {0};
    char Keyword[61] = {0};
    int Inventory = 0;
    double Price = 0;

    Book() = default;
    Book(std::string& ISBN);
};

class BookSystem {
public:
    memory ISBN_file;
    memory BookName_file;
    memory Author_file;
    memory Keyword_file;
    MemoryRiver<Book> book_river_;

    BookSystem();
    void show(std::string& input);
    void buy(std::string& ISBN, std::string& Quantity, Diary& diary);
    void select(std::string& ISBN, LoginStack& login_stack);
    void modify(std::vector<std::string>& input, LoginStack& login_stack);
    void import(std::string& Quantity, std::string& TotalCost, LoginStack& login_stack, Diary& diary);
};
#endif //BOOK_HPP
