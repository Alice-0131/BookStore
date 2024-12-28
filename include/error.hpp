#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include <exception>
#include <vector>

class ErrorException : public std::exception {
public:
    explicit ErrorException(std::string message);

    std::string getMessage() const;

private:
    std::string message;
};

void error(std::string message);

class Check {
public:
    static void checkAccount1(std::string& input); //[UserID], [Password], [CurrentPassword], [NewPassword]
    static void checkAccount2(std::string& input); //[Username]
    static int checkAccount3(std::string& input); //[Privilege]
    static void checkBook1(std::string& input); //[ISBN]
    static void checkBook2(std::string& input); //[BookName], [Author]
    static void checkBook3(std::string& input); //单个[Keyword]
    static std::vector<std::string> checkKeyword(const std::string& input, bool flag); //多个[Keyword]
    static int checkBook4(std::string& input); //[Quantity]
    static double checkBook5(std::string& input); //[Price], [TotalCost]
};


#endif //ERROR_HPP
