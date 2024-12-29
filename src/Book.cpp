#include "../include/Book.hpp"
#include "../include/error.hpp"
#include "../include/File.hpp"
#include "../include/Diary.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

bool cmp(const Book& a, const Book& b) {
    if (strcmp(a.ISBN, b.ISBN) < 0) {
        return true;
    } else {
        return false;
    }
}

Book::Book(std::string& ISBN) {
    strcpy(this->ISBN, ISBN.c_str());
}

BookSystem::BookSystem():
ISBN_file("ISBN", "ISBN_no"),
BookName_file("BookName", "BookName_no"),
Author_file("Author", "Author_no"),
Keyword_file("Keyword", "Keyword_no")
{
    book_river_.initialise("book");
}

void BookSystem::end() {
    ISBN_file.end();
    BookName_file.end();
    Author_file.end();
    Keyword_file.end();
}

void BookSystem::show(std::string &input) {
    std::vector<int> no;
    if (input.empty()) {
        no = ISBN_file.readAll();
    } else {
        std::string type;
        int i = 0;
        for (; i < input.length(); ++i) {
            if (input[i] != '=') {
                type += input[i];
            } else {
                break;
            }
        }
        ++i;
        if (type == "-ISBN") {
            std::string ISBN;
            for (; i < input.length(); ++i) {
                ISBN += input[i];
            }
            Check::checkBook1(ISBN);
            no = ISBN_file.Find(ISBN);
        } else if (type == "-name") {
            std::string name;
            if (input[i] != '"' || input[input.length() - 1] != '"') {
                error("Invalid\n");
            }
            ++i;
            for (; i < input.length() - 1; ++i) {
                name += input[i];
            }
            Check::checkAccount2(name);
            no = BookName_file.Find(name);
        } else if (type == "-author") {
            std::string author;
            if (input[i] != '"' || input[input.length() - 1] != '"') {
                error("Invalid\n");
            }
            ++i;
            for (; i < input.length() - 1; ++i) {
                author += input[i];
            }
            Check::checkBook2(author);
            no = Author_file.Find(author);
        } else if (type == "-keyword") {
            std::string keyword;
            if (input[i] != '"' || input[input.length() - 1] != '"') {
                error("Invalid\n");
            }
            ++i;
            for (; i < input.length() - 1; ++i) {
                keyword += input[i];
            }
            Check::checkBook3(keyword);
            no = Keyword_file.Find(keyword);
        } else {
            error("Invalid\n");
        }
    }
    std::vector<Book> books;
    Book tmp;
    for (int i = 0; i < no.size(); ++i) {
        book_river_.read(tmp, no[i]);
        books.push_back(tmp);
    }
    std::sort(books.begin(), books.end(), cmp);
    if (books.empty()) {
        std::cout << '\n';
    } else {
        for (int i = 0; i < books.size(); ++i) {
            std::cout << books[i].ISBN << '\t' << books[i].BookName << '\t' << books[i].Author << '\t';
            std::cout << books[i].Keyword << '\t' << std::fixed << std::setprecision(2) << books[i].Price << '\t';
            std::cout << books[i].Inventory << '\n';
        }
    }
}

void BookSystem::buy(std::string &ISBN, std::string &Quantity, Diary& diary) {
    Check::checkBook1(ISBN);
    int quantity = Check::checkBook4(Quantity);
    std::vector<int> no;
    no = ISBN_file.Find(ISBN);
    if (no.empty()) {
        error("Invalid\n");
    }
    Book book;
    book_river_.read(book, no[0]);
    if (quantity > book.Inventory) {
        error("Invalid\n");
    }
    Deal deal(true, book.Price * quantity);
    int size;
    diary.deal_river_.get_info(size, 1);
    diary.deal_river_.write(deal, size);
    ++size;
    diary.deal_river_.write_info(size, 1);
    std::cout << std::fixed << std::setprecision(2) << book.Price * quantity << '\n';
    book.Inventory -= quantity;
    book_river_.write(book, no[0]);
}

void BookSystem::select(std::string &ISBN, LoginStack &login_stack) {
    Check::checkBook1(ISBN);
    std::vector<int> no;
    no = ISBN_file.Find(ISBN);
    if (no.empty()) {
        Book book(ISBN);
        int size;
        book_river_.get_info(size, 1);
        book_river_.write(book, size);
        ISBN_file.Insert(size, ISBN);
        no.push_back(size);
        ++size;
        book_river_.write_info(size, 1);
    }
    login_stack.stack[login_stack.stack.size() - 1].second = no[0];
}

void BookSystem::modify(std::vector<std::string> &input, LoginStack& login_stack) {
    int book_no = login_stack.stack[login_stack.stack.size() - 1].second;
    if (book_no == -1) {
        error("Invalid\n");
    }
    bool flag_I = false, flag_n = false, flag_a = false, flag_k = false, flag_p = false;
    std::string ISBN, name, author, keyword, price;
    std::vector<std::string> keywords;
    double PRICE;
    if (input.empty()) {
        error("Invalid\n");
    }
    for (int i = 0; i < input.size(); ++i) {
        std::string type;
        int j = 0;
        for (; j < input[i].length(); ++j) {
            if (input[i][j] != '=') {
                type += input[i][j];
            } else {
                break;
            }
        }
        ++j;
        if (type == "-ISBN") {
            if (flag_I) {
                error("Invalid\n");
            }
            flag_I = true;
            for (; j < input[i].length(); ++j) {
                ISBN += input[i][j];
            }
            Check::checkBook1(ISBN);
        } else if (type == "-name") {
            if (flag_n) {
                error("Invalid\n");
            }
            flag_n = true;
            if (input[i][j] != '"' || input[i][input[i].length() - 1] != '"') {
                error("Invalid\n");
            }
            ++j;
            for (; j < input[i].length() - 1; ++j) {
                name += input[i][j];
            }
            Check::checkAccount2(name);
        } else if (type == "-author") {
            if (flag_a) {
                error("Invalid\n");
            }
            flag_a = true;
            if (input[i][j] != '"' || input[i][input[i].length() - 1] != '"') {
                error("Invalid\n");
            }
            ++j;
            for (; j < input[i].length() - 1; ++j) {
                author += input[i][j];
            }
            Check::checkBook2(author);
        } else if (type == "-keyword") {
            if (flag_k) {
                error("Invalid\n");
            }
            flag_k = true;
            if (input[i][j] != '"' || input[i][input[i].length() - 1] != '"') {
                error("Invalid\n");
            }
            ++j;
            for (; j < input[i].length() - 1; ++j) {
                keyword += input[i][j];
            }
            keywords = Check::checkKeyword(keyword, false);
        } else if (type == "-price") {
            if (flag_p) {
                error("Invalid\n");
            }
            flag_p = true;
            for (; j < input[i].length(); ++j) {
                price += input[i][j];
            }
            PRICE = Check::checkBook5(price);
        } else {
            error("Invalid\n");
        }
    }
    Book book;
    book_river_.read(book, book_no);
    if (flag_I) {
        if (ISBN == book.ISBN) {
            error("Invalid\n");
        }
        std::vector<int> no = ISBN_file.Find(ISBN);
        if (!no.empty()) {
            error("Invalid\n");
        }
        ISBN_file.Delete(book_no, book.ISBN);
        ISBN_file.Insert(book_no, ISBN);
        strcpy(book.ISBN, ISBN.c_str());
    }
    if (flag_n) {
        BookName_file.Delete(book_no, book.BookName);
        BookName_file.Insert(book_no, name);
        strcpy(book.BookName, name.c_str());
    }
    if (flag_a) {
        Author_file.Delete(book_no, book.Author);
        Author_file.Insert(book_no, author);
        strcpy(book.Author, author.c_str());
    }
    if (flag_k) {
        std::vector<std::string> pre_keywords = Check::checkKeyword(book.Keyword, true);
        for (int i = 0; i < pre_keywords.size(); ++i) {
            Keyword_file.Delete(book_no, pre_keywords[i]);
        }
        for (int i = 0; i < keywords.size(); ++i) {
            Keyword_file.Insert(book_no, keywords[i]);
        }
        strcpy(book.Keyword, keyword.c_str());
    }
    if (flag_p) {
        book.Price = PRICE;
    }
    book_river_.write(book, book_no);
}

void BookSystem::import(std::string &Quantity, std::string &TotalCost, LoginStack& login_stack, Diary& diary) {
    int quantity = Check::checkBook4(Quantity);
    double total_cost = Check::checkBook5(TotalCost);
    int book_no = login_stack.stack[login_stack.stack.size() - 1].second;
    if (book_no == -1) {
        error("Invalid\n");
    }
    Book book;
    book_river_.read(book, book_no);
    book.Inventory += quantity;
    book_river_.write(book, book_no);
    Deal deal(false, total_cost);
    int size;
    diary.deal_river_.get_info(size, 1);
    diary.deal_river_.write(deal, size);
    ++size;
    diary.deal_river_.write_info(size, 1);
}
