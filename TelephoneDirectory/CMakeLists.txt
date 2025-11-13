cmake_minimum_required(VERSION 3.31)
project(TelephoneDirectory)

set(CMAKE_CXX_STANDARD 26)

add_executable(TelephoneDirectory
        main.cpp
        Contact.cpp
        Contact.h
        BirthDate.cpp
        BirthDate.h
        ContactManager.cpp
        ContactManager.h)
