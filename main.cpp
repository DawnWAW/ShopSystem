#include <iostream>
#include <ItemService.h>
#include <Logio.h>
#include <MainMenu.h>

int main() {
    // Database test
    try {
        Database database("shop.db");
        Logio logio(database);
        MainMenu mainMenu(database,logio);
        mainMenu.run();

    }catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
