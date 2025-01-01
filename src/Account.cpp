#include "../include/Account.hpp"
#include "../include/error.hpp"
#include "../include/File.hpp"
#include "../include/Diary.hpp"
#include <string>
#include <vector>
#include <cstring>

Account::Account(const std::string& UserID, const std::string& Password, const std::string& Username):
Privilege(1){
    strcpy(this->UserID, UserID.c_str());
    strcpy(this->Password, Password.c_str());
    strcpy(this->Username, Username.c_str());
}

Account::Account(const std::string& UserID, const std::string& Password, int Privilege):Privilege(Privilege) {
    strcpy(this->UserID, UserID.c_str());
    strcpy(this->Password, Password.c_str());
}

Account::Account(const std::string& UserID, const std::string& Password, int Privilege, const std::string& Username):
Privilege(Privilege) {
    strcpy(this->UserID, UserID.c_str());
    strcpy(this->Password, Password.c_str());
    strcpy(this->Username, Username.c_str());
}


int LoginStack::getPrivilege() {
    if (stack.empty()) {
        return 0;
    } else {
        Account acc;
        account_river_.read(acc, stack[stack.size() - 1].first);
        return acc.Privilege;
    }
}

LoginStack::LoginStack() :UserID_file("UserID", "UserID_no"){
    account_river_.initialise("account");
}

void LoginStack::end() {
    UserID_file.end();
}

void LoginStack::su(std::string &UserID, std::string &Password) {
    Check::checkAccount1(UserID);
    Check::checkAccount1(Password);
    std::vector<int> no = UserID_file.Find(UserID);
    if (no.empty()) { // 账户不存在
        error("Invalid\n");
    }
    Account acc;
    account_river_.read(acc, no[0]);
    if (strcmp(acc.Password, Password.c_str()) != 0) { // 密码错误
        error("Invalid\n");
    }
    stack.emplace_back(no[0], -1);
    privilege = acc.Privilege;
}

void LoginStack::su(std::string &UserID) {
    Check::checkAccount1(UserID);
    std::vector<int> no = UserID_file.Find(UserID);
    if (no.empty()) { // 账户不存在
        error("Invalid\n");
    }
    Account acc;
    account_river_.read(acc, no[0]);
    if (privilege <= acc.Privilege) {
        error("Invalid\n");
    }
    stack.emplace_back(no[0], -1);
    privilege = acc.Privilege;
}


void LoginStack::logout() {
    if (stack.empty()) {
        error("Invalid\n");
    }
    stack.pop_back();
    privilege = getPrivilege();
}

void LoginStack::Register(std::string &UserID, std::string &Password, std::string &Username) {
    Check::checkAccount1(UserID);
    Check::checkAccount1(Password);
    Check::checkAccount2(Username);
    std::vector<int> no = UserID_file.Find(UserID);
    if (!no.empty()) { // 账户已存在
        error("Invalid\n");
    }
    Account acc(UserID, Password, Username);
    int size;
    account_river_.get_info(size, 1);
    account_river_.write(acc, size);
    UserID_file.Insert(size, UserID);
    ++size;
    account_river_.write_info(size, 1);
}

void LoginStack::passwd(std::string &UserID, std::string &CurrentPassword, std::string &NewPassword) {
    Check::checkAccount1(UserID);
    Check::checkAccount1(CurrentPassword);
    Check::checkAccount1(NewPassword);
    if (privilege < 1) { // 权限不够
        error("Invalid\n");
    }
    std::vector<int> no = UserID_file.Find(UserID);
    if (no.empty()) { // 账户不存在
        error("Invalid\n");
    }
    Account acc;
    account_river_.read(acc, no[0]);
    if (strcmp(acc.Password, CurrentPassword.c_str()) != 0) { // 密码错误
        error("Invalid\n");
    }
    strcpy(acc.Password, NewPassword.c_str());
    account_river_.write(acc, no[0]);
}

void LoginStack::passwd(std::string &UserID, std::string &NewPassword) {
    Check::checkAccount1(UserID);
    Check::checkAccount1(NewPassword);
    if (privilege < 7) { // 权限不够
        error("Invalid\n");
    }
    std::vector<int> no = UserID_file.Find(UserID);
    if (no.empty()) { // 账户不存在
        error("Invalid\n");
    }
    Account acc;
    account_river_.read(acc, no[0]);
    strcpy(acc.Password, NewPassword.c_str());
    account_river_.write(acc, no[0]);
}

void LoginStack::useradd(std::string &UserID, std::string &Password, std::string &Privilege, std::string &Username, Diary& diary) {
    Check::checkAccount1(UserID);
    Check::checkAccount1(Password);
    int Privilege_ = Check::checkAccount3(Privilege);
    Check::checkAccount2(Username);
    if (Privilege_ >= privilege || privilege < 3) { // 权限不够
        error("Invalid\n");
    }
    std::vector<int> no = UserID_file.Find(UserID);
    if (!no.empty()) { // 账户已存在
        error("Invalid\n");
    }
    Account acc(UserID, Password, Privilege_, Username);
    int size;
    account_river_.get_info(size, 1);
    account_river_.write(acc, size);
    UserID_file.Insert(size, UserID);
    ++size;
    account_river_.write_info(size, 1);
    Account account;
    account_river_.read(account, stack[stack.size() - 1].first);
    Operation operation(account.UserID, "account", acc.UserID, "useradd", Username);
    diary.operation_river_.get_info(size, 1);
    diary.operation_river_.write(operation, size);
    ++size;
    diary.operation_river_.write_info(size, 1);
}

void LoginStack::Delete(std::string &UserID) {
    Check::checkAccount1(UserID);
    if (privilege < 7) { // 权限不够
        error("Invalid\n");
    }
    std::vector<int> no = UserID_file.Find(UserID);
    if (no.empty()) { // 账户不存在
        error("Invalid\n");
    }
    for (auto & i : stack) {
        if (i.first == no[0]) { // 待删除账户已登录
            error("Invalid\n");
        }
    }
    UserID_file.Delete(no[0], UserID);
}

void LoginStack::createroot() {
    Account root("root", "sjtu", 7);
    int size;
    account_river_.get_info(size, 1);
    if (!size) {
        account_river_.write(root, size);
        UserID_file.Insert(size, "root");
        ++size;
        account_river_.write_info(size, 1);
    }
}
