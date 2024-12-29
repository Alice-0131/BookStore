#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP
#include "File.hpp"
#include <string>
#include <vector>

struct Account {
    char UserID[31] = {0};
    char Password[31] = {0};
    char Username[31] = {0};
    int Privilege = 0;

    Account() = default;
    Account(const std::string& UserID, const std::string& Password, const std::string& Username);
    Account(const std::string& UserID, const std::string& Password, int Privilege);
    Account(const std::string& UserID, const std::string& Password, int Privilege, const std::string& Username);
};

class LoginStack {
    friend class BookSystem;
private:
    std::vector<std::pair<int, int>> stack; // No_of_Account,No_of_Book
    int privilege = 0;
    memory UserID_file;
    MemoryRiver<Account> account_river_;

public:
    int getPrivilege();
    LoginStack();
    void end();
    void su(std::string& UserID, std::string& Password);
    void su(std::string& UserID);
    void logout();
    void Register(std::string& UserID, std::string& Password, std::string& Username);
    void passwd(std::string& UserID, std::string& CurrentPassword, std::string& NewPassword);
    void passwd(std::string& UserID, std::string& NewPassword);
    void useradd(std::string& UserID, std::string& Password, std::string& Privilege, std::string& Username);
    void Delete(std::string& UserID);
    void createroot();
};

#endif //ACCOUNT_HPP
