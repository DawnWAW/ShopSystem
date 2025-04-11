//
// Created by dawn on 2025/4/8.
//

#include "MainMenu.h"

#include <ItemService.h>
#include <ShopMenu.h>


MainMenu::MainMenu(Database &database,Logio &logio, ItemService &item_service) : Menu("MainMenu"), database(database), logio(logio),item_service(item_service) {
    this->updateMainMenu();
}

void MainMenu::showAppState() const {
    if (this->appState.isAdmin) {
        std::cout <<"[Admin]";
    }
    else {
        std::cout <<"["<<this->appState.account.getUsername()<<"]";
    }
    if (this->appState.isLoggedIn) {
        std::cout <<"(LoggedIn)";
    }
    else {
        std::cout <<"(Not LoggedIn)";
    }
    std::cout << std::endl;
}


void MainMenu::updateMainMenu() {
    this->clearItem();
    if(this->appState.isLoggedIn) {
        this->addItem("Shopping",
            [this](){
                ShopMenu shop_menu(item_service,1);
                shop_menu.addItem("Cart item",
                    [](){ std::cout << "Under development" << std::endl; });
                shop_menu.addItem("Next page",
                    [&shop_menu]() {
                        shop_menu.nextPage();
                    });
                shop_menu.addItem("Previous page",
                    [&shop_menu]() {
                        shop_menu.prevPage();
                    });
                shop_menu.run();
         });
        if (this->appState.isAdmin) {
            this->addItem("Items Management",
                [this]() {
                    Menu item_management_menu("Items Management");
                    item_management_menu.addItem("Show all items",
                                                 [this]() {
                                                     item_service.showAllItems(true);
                                                 });
                    item_management_menu.addItem("Add item",
                                                 [this]() {
                                                     item_service.addItem();
                                                 });
                    item_management_menu.addItem("Update item",
                                                 [this]() {
                                                     item_service.updateItem();
                                                 });
                    item_management_menu.addItem("Delete item",
                        [this]() {
                            item_service.deleteItem();
                        });
                    item_management_menu.run();
                });
        }
        this->addItem("Change Password",
            [this](){ logio.changePassword(); });
        this->addItem("Logout",
            [this]() {
                logio.logout();
                this->appState.account = logio.current_account();
                this->appState.isLoggedIn=false;
                this->appState.isAdmin=false;
                this->updateMainMenu();
                this->showAppState();
            });
    }
    else {
        this->addItem("Browsing",
            [this](){
            ShopMenu shop_menu(item_service,0);
                shop_menu.addItem("Cart item",
                    [](){ std::cout << "Under development" << std::endl; });
                shop_menu.addItem("Next page",
                    [&shop_menu]() {
                        shop_menu.nextPage();
                    });
                shop_menu.addItem("Previous page",
                    [&shop_menu]() {
                        shop_menu.prevPage();
                    });
                shop_menu.run();
        });
        this->addItem("Login",
            [this]() {
                if (logio.login()) {
                    this->appState.isLoggedIn=true;
                    this->appState.account = logio.current_account();
                    if (logio.current_account().getUsername() == "root") {
                        this->appState.isAdmin=true;
                    }
                    this->updateMainMenu();
                    this->showAppState();
                }
            });
    }
}


