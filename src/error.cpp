#include <string>
#include "../include/error.hpp"

ErrorException::ErrorException(std::string message) {
    this -> message = message;
}

std::string ErrorException::getMessage() const {
    return message;
}

void error(std::string message) {
    throw ErrorException(message);
}

void Check::checkAccount1(std::string &input) {
    if (input.length() > 30 || input.empty()) {
        error("Invalid\n");
    }
    for (int i = 0; i < input.length(); ++i) {
        if (!(input[i] >= '0' && input[i] <= '9' || input[i] >= 'a' && input[i] <= 'z'
            || input[i] >= 'A' && input[i] <= 'Z' || input[i] == '_')) {
            error("Invalid\n");
        }
    }
}

void Check::checkAccount2(std::string &input) {// TODO:不可见字符
    if (input.length() > 30 || input.empty()) {
        error("Invalid\n");
    }
}

int Check::checkAccount3(std::string &input) {
    if (input.length() > 1 || input.empty()) {
        error("Invalid\n");
    }
    if (input[0] != '1' && input[0] != '3' && input[0] != '7') {
        error("Invalid\n");
    }
    return input[0] - '0';
}

void Check::checkBook1(std::string &input) { // TODO:不可见字符
    if (input.length() > 20 || input.empty()) {
        error("Invalid\n");
    }
}

void Check::checkBook2(std::string &input) { // TODO:不可见字符
    if (input.length() > 60 || input.empty()) {
        error("Invalid\n");
    }
    for (int i = 0; i < input.length(); ++i) {
        if (input[i] == '"') {
            error("Invalid\n");
        }
    }
}

void Check::checkBook3(std::string &input) { // TODO:不可见字符
    if (input.length() > 60 || input.empty()) {
        error("Invalid\n");
    }
    for (int i = 0; i < input.length(); ++i) {
        if (input[i] == '"') {
            error("Invalid\n");
        }
    }
}

std::vector<std::string> Check::checkKeyword(const std::string &input, bool flag) { // TODO:不可见字符
    if (!flag) {
        if (input.length() > 60 || input.empty()) {
            error("Invalid\n");
        }
    } else {
        if (input.length() > 60) {
            error("Invalid\n");
        }
    }
    std::vector<std::string> keywords;
    std::string keyword;
    for (int i = 0; i < input.length(); ++i) {
        if (input[i] == '"') {
            error("Invalid\n");
        }
        if (input[i] == '|') {
            for (int j = 0; j < keywords.size(); ++j) {
                if (keyword == keywords[j]) {
                    error("Invalid\n");
                }
            }
            keywords.push_back(keyword);
            keyword.erase(keyword.begin(), keyword.end());
        } else {
            keyword += input[i];
        }
    }
    for (int j = 0; j < keywords.size(); ++j) {
        if (keyword == keywords[j]) {
            error("Invalid\n");
        }
    }
    keywords.push_back(keyword);
    return keywords;
}

int Check::checkBook4(std::string &input) {
    if (input.length() > 10) {
        error("Invalid\n");
    }
    int Quantity = 0;
    for (int i = 0; i < input.length(); ++i) {
        if (input[i] > '9' || input[i] < '0') {
            error("Invalid\n");
        }
        Quantity *= 10;
        Quantity += input[i] - '0';
    }
    if (!Quantity) {
        error("Invalid\n");
    }
    return Quantity;
}

double Check::checkBook5(std::string &input) {
    if (input.length() > 13) {
        error("Invalid\n");
    }
    int flag = input.length();
    if (input[0] == '.' || input[input.length() - 1] == '.') {
        error("Invalid\n");
    }
    for (int i = 0; i < input.length(); ++i) {
        if (input[i] > '9' || input[i] < '0') {
            if (input[i] == '.') {
                if (flag != input.length()) {
                    error("Invalid\n");
                } else {
                    flag = i;
                }
            } else {
                error("Invalid\n");
            }
        }
    }
    double output = 0;
    for (int i = 0; i < flag; ++i) {
        output *= 10;
        output += input[i] - '0';
    }
    ++flag;
    if (flag < input.length()) {
        output += static_cast<double>(input[flag] - '0') / 10;
        ++flag;
        if (flag < input.length()) {
            output += static_cast<double>(input[flag] - '0') / 100;
        }
    }
    return output;
}

