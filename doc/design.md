# Bookstore 总体开发文档
> * 项目名称：Bookstore
> * 文档作者：许佳琪
---
## 程序功能概述
- 管理账户系统
    - 维护账户信息
    - 实现账户相关指令
- 管理图书系统
    - 维护图书信息
    - 实现图书相关指令
- 管理日志系统
    - 维护日志信息
    - 实现日志相关指令
---
## 主体逻辑说明
开始程序  
循环{  
读取指令  
若(指令不合法) 抛出错误  
否则 执行指令  
}

---
## 代码文件结构
### main.cpp
作为程序的主入口点，负责启动程序、调用各种指令。
### File.hpp/File.cpp
包含文件读写、数据存储相关的函数。
### Account.hpp/Account.cpp
包含实现账户相关指令的函数。
### Book.hpp/Book.cpp
包含实现图书相关指令的函数。
### Diary.hpp/Diary.cpp
包含实现日志相关指令的函数。
### TokenScanner.hpp/TokenScanner.cpp
包含将指令分块的函数。
### error.hpp/error.cpp
包含检查指令合法性、抛出错误的函数。

---
## 功能设计
### 基础模块
- quit/exit：退出系统
### 账户模块
- 游客
  - 登录账户：使登录帐户变为已登录状态，当前帐户变为该帐户
  - 注册账户：注册权限等级为{1}的账户
- 顾客
  - 注销账户：撤销最后一次成功执行的登录指令效果
  - 修改密码：修改指定帐户的密码
- 员工
  - 创建账户：创建一个账户
- 店主
  - 删除账户：删除特定账户
### 图书模块
- 顾客
  - 检索图书：以 [ISBN] 字典升序依次输出满足要求的图书信息
  - 购买图书：购买指定数量的指定图书，以浮点数输出购买图书所需的总金额。
- 员工
  - 选择图书：以当前帐户选中指定图书
  - 修改图书信息：以指令中的信息更新选中图书的信息
  - 图书进货：以指定交易总额购入指定数量的选中图书，增加其库存数
### 日志模块
- 店主
  - 财务纪录查询
  - 生成财务纪录报告
  - 生成全体员工工作情况报告
  - 生成日志

![](功能结构图.png)

---
## 数据库设计
### 账户信息
- 需要存储的信息：
  - [UserID]（独特）, [Password]  
    合法字符集：数字，字母，下划线；   
    最大长度：30。
  - [Username]  
    合法字符集：除不可见字符以外 ASCII 字符；  
    最大长度：30。
  - [Privilege]  
    合法字符集：数字；  
    最大长度：1。
- 存储方式：UserID存头结点，UserID存块，account存所有账户信息
### 图书信息
- 需要存储的信息：
  - [ISBN]（独特）  
    合法字符集：除不可见字符以外 ASCII 字符；  
    最大长度：20；
  - [BookName], [Author]  
    合法字符集：除不可见字符和英文双引号以外 ASCII 字符；  
    最大长度：60。
  - [Keyword]  
    合法字符集：除不可见字符和英文双引号以外 ASCII 字符；  
    最大长度：60；  
    特殊说明：[keyword] 内容以 | 为分隔可以出现多段信息。
  - [Inventory]  
    合法字符集：数字；  
    最大长度：10；  
    特殊说明：数值不超过 2'147'483'647。
  - [Price]  
    合法字符集：数字和 .；  
    最大长度：13；  
    特殊说明：本系统中浮点数输入输出精度固定为小数点后两位。
- 存储方式：
  - ISBN存头结点，ISBN-no存块
  - BookName存头结点，BookName-no存块
  - Author存头结点，Author-no.txt存块
  - Keyword存头结点，Keyword-no存块
### 日志信息
- 需要存储的信息：
  - 交易金额：  
    支出/收入  
    金额大小
  - 员工工作情况：  
    操作类型  
    操作内容
- 存储方式：
  - deal存财务纪录
  - employee存头结点，employee-no存员工工作情况
---
## 类、结构体设计
### Account结构体
用于存储Account的各种信息
```cpp
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
```
### LoginStack类
用于处理登录栈、账户系统
```cpp
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
    void useradd(std::string& UserID, std::string& Password, std::string& Privilege, std::string& Username, Diary& diary);
    void Delete(std::string& UserID);
    void createroot();
};
```
### Book结构体
用于存储Book的各种信息
```cpp
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
```
### BookSystem类
用于处理图书管理系统
```cpp
BookSystem();
void end();
void show(std::string& input);
void buy(std::string& ISBN, std::string& Quantity, Diary& diary);
void select(std::string& ISBN, LoginStack& login_stack);
void modify(std::vector<std::string>& input, LoginStack& login_stack, Diary& diary);
void import(std::string& Quantity, std::string& TotalCost, LoginStack& login_stack, Diary& diary);
```
### Deal结构体
用于存储每一笔交易的信息
```cpp
struct Deal {
    bool sign = false;
    double money = 0;
    char book[21] = {0};

    Deal() = default;
    Deal(bool sign, double money, char book[]);
};
```
### Operation结构体
用于存储员工的操作信息
```cpp
struct Operation {
    char UserID[31] = {0};
    char type[10] = {0};
    char Object[31] = {0};
    char Order[31] = {0};
    char Content[75] = {0};

    Operation(char UserID[], std::string type, char Object[], std::string Order, std::string& Content);
    Operation() = default;
};
```
### Diary类
用于处理日志系统
```cpp
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
```
### ErrorException类
用于处理错误信息
```cpp
class ErrorException : public std::exception {
public:
    explicit ErrorException(std::string message);

    std::string getMessage() const;

private:
    std::string message;
};
```
### Check类
用于检查指令合法性
```cpp
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
```
### Memory类、MemoryRiver类
对不同对象进行存储，实现文件读写
### TokenScanner类
用于将指令分块
```cpp
class TokenScanner {
public:
    TokenScanner(std::string& input);
    std::string nextToken();
    bool hasMoreTokens();

private:
    std::string str;
    int index;

    void skipWhitespaces();
};
```

---
## 其他补充说明
无