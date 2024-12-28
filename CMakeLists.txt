cmake_minimum_required(VERSION 3.22)
project(Bookstore-2024)

set(CMAKE_CXX_STANDARD 20)

add_executable(code
        src/main.cpp
        include/File.hpp
        src/File.cpp
        include/Account.hpp
        src/Account.cpp
        src/Book.cpp
        include/Book.hpp
        include/Diary.hpp
        src/Diary.cpp
        include/error.hpp
        src/error.cpp
        include/TokenScanner.hpp
        src/TokenScanner.cpp)
