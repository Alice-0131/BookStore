#include <iostream>
#include <string>
#include <vector>

#include "../include/Account.hpp"
#include "../include/Book.hpp"
#include "../include/error.hpp"
#include "../include/File.hpp"
#include "../include/TokenScanner.hpp"

void processLine(std::string& line, LoginStack& login_stack, BookSystem& book_system);

memory ISBN("ISBN", "ISBN_no");
memory BookName("BookName", "BookName_no");
memory Author("Author", "Author_no");
memory Keyword("Keyword", "Keyword_no");

int main() {
    LoginStack login_stack;
    BookSystem book_system;
    login_stack.createroot("root", "sjtu", 7);
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty()) {
                continue;
            }
            processLine(input, login_stack, book_system);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage();
        }
    }

    return 0;
}

void processLine(std::string& line, LoginStack& login_stack, BookSystem& book_system) {
    TokenScanner scanner(line);
    std::string token = scanner.nextToken();
    if (token.empty()) {
        return;
    }
    if (token == "exit" || token == "quit") {
        exit(0);
    } else if (token == "su") {
        std::string UserID = scanner.nextToken();
        if (scanner.hasMoreTokens()) {
            std::string PassWord = scanner.nextToken();
            if (scanner.hasMoreTokens()) {
                error("Invalid\n");
            }
            login_stack.su(UserID, PassWord);
        } else {
            login_stack.su(UserID);
        }
    } else if (token == "logout") {
        if (scanner.hasMoreTokens()) {
            error("Invalid\n");
        }
        login_stack.logout();
    } else if (token == "register") {
        std::string UserID = scanner.nextToken();
        std::string Password = scanner.nextToken();
        std::string UserName = scanner.nextToken();
        if (scanner.hasMoreTokens()) {
            error("Invalid\n");
        }
        login_stack.Register(UserID, Password, UserName);
    } else if (token == "passwd") {
        std::string UserID = scanner.nextToken();
        std::string Password = scanner.nextToken();
        if (scanner.hasMoreTokens()) {
            std::string NewPassword = scanner.nextToken();
            if (scanner.hasMoreTokens()) {
                error("Invalid\n");
            }
            login_stack.passwd(UserID, Password, NewPassword);
        } else {
            login_stack.passwd(UserID, Password);
        }
    } else if (token == "useradd") {
        std::string UserID = scanner.nextToken();
        std::string Password = scanner.nextToken();
        std::string Privilege = scanner.nextToken();
        std::string Username = scanner.nextToken();
        if (scanner.hasMoreTokens()) {
            error("Invalid\n");
        }
        login_stack.useradd(UserID, Password, Privilege, Username);
    } else if (token == "delete") {
        std::string UserID = scanner.nextToken();
        if (scanner.hasMoreTokens()) {
            error("Invalid\n");
        }
        login_stack.Delete(UserID);
    } else if (token == "show") {
        token = scanner.nextToken();
        if (token == "finance") {

        } else {
            if (login_stack.getPrivilege() < 1) {
                error("Invalid\n");
            }
            book_system.show(token);
        }
    } else if (token == "buy") {
        if (login_stack.getPrivilege() < 1) {
            error("Invalid\n");
        }
        std::string ISBN = scanner.nextToken();
        std::string Quantity = scanner.nextToken();
        book_system.buy(ISBN, Quantity);
    } else if (token == "select") {
        if (login_stack.getPrivilege() < 3) {
            error("Invalid\n");
        }
        std::string ISBN = scanner.nextToken();
        book_system.select(ISBN, login_stack);
    } else if (token == "modify") {
        if (login_stack.getPrivilege() < 3) {
            error("Invalid\n");
        }
        std::vector<std::string> input;
        std::string tmp;
        while (scanner.hasMoreTokens()) {
            tmp = scanner.nextToken();
            input.push_back(tmp);
        }
        book_system.modify(input, login_stack);
    } else if (token == "import") {
        if (login_stack.getPrivilege() < 3) {
            error("Invalid\n");
        }
        std::string Quantity = scanner.nextToken();
        std::string TotalCost = scanner.nextToken();
        book_system.import(Quantity, TotalCost, login_stack);
    } else if (token == "log") {

    } else if (token == "report") {
        token = scanner.nextToken();
        if (token == "finance") {

        } else if (token == "employee") {

        }
    } else {
        error("Invalid\n");
    }
}
