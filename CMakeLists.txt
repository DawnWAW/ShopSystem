cmake_minimum_required(VERSION 3.30)
project(ShopSystem)

set(CMAKE_CXX_STANDARD 20)

# 查找 SQLite3 库
find_package(SQLite3 REQUIRED)

set(INCLUDE include)
set(MODULE src/module)

add_executable(ShopSystem main.cpp
        ${MODULE}/Account/Account.cpp
        ${MODULE}/Account/Logio.cpp
        ${MODULE}/Shop/Item.cpp
        ${MODULE}/Shop/Cart.cpp
        ${MODULE}/Shop/ItemService.cpp
        ${MODULE}/Menu/Menu.cpp
        ${MODULE}/Menu/MainMenu.cpp
        ${MODULE}/Menu/FormMenu.cpp
        ${MODULE}/Menu/ShopMenu.cpp
        ${MODULE}/Data/Database.cpp
        ${MODULE}/Order/Order.cpp
)

add_executable(database_order_test test/database/database_order_test.cpp
        ${MODULE}/Account/Account.cpp
        ${MODULE}/Account/Logio.cpp
        ${MODULE}/Shop/Item.cpp
        ${MODULE}/Shop/Cart.cpp
        ${MODULE}/Shop/ItemService.cpp
        ${MODULE}/Menu/Menu.cpp
        ${MODULE}/Menu/MainMenu.cpp
        ${MODULE}/Menu/FormMenu.cpp
        ${MODULE}/Menu/ShopMenu.cpp
        ${MODULE}/Data/Database.cpp
        ${MODULE}/Order/Order.cpp
)

target_include_directories(ShopSystem PUBLIC ${INCLUDE})
target_include_directories(database_order_test PUBLIC ${INCLUDE})


# 在你的可执行目标中添加链接
target_link_libraries(ShopSystem PRIVATE SQLite::SQLite3)
target_link_libraries(database_order_test PRIVATE SQLite::SQLite3)

