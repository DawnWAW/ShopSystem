#include <iostream>
#include <MainMenu.h>

#include "include/Database.h"

int main() {
    // Database test
    try {
        Database database("shop.db");
        MainMenu mainMenu(database);
        mainMenu.showMainMenu();

    }catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
