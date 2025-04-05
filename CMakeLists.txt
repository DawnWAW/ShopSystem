cmake_minimum_required(VERSION 3.30)
project(ShopSystem)

set(CMAKE_CXX_STANDARD 20)

# 查找 SQLite3 库
find_package(SQLite3 REQUIRED)

# 在你的可执行目标中添加链接

add_executable(ShopSystem main.cpp
        src/module/Account/Account.cpp
        include/Account.h
        src/module/Shop/Item.cpp
        include/Item.h
        src/module/Shop/Cart.cpp
        include/Cart.h
        src/module/Menu/Menu.cpp
        include/Menu.h
        src/module/Data/Database.cpp
        include/Database.h)

target_link_libraries(ShopSystem PRIVATE SQLite::SQLite3)
