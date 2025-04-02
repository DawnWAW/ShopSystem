cmake_minimum_required(VERSION 3.30)
project(ShopSystem)

set(CMAKE_CXX_STANDARD 20)

add_executable(ShopSystem main.cpp
        src/module/Accout/Account.cpp
        src/module/Accout/Account.h
        src/module/Shop/Item.cpp
        src/module/Shop/Item.h
        src/module/Shop/Cart.cpp
        src/module/Shop/Cart.h
        src/module/Menu/Menu.cpp
        src/module/Menu/Menu.h
        src/module/Data/Database.cpp
        src/module/Data/Database.h)
