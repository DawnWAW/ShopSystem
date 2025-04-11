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

void MainMenu::showItemService(bool isLoggedIn) const {
    int view_state = isLoggedIn ? 1 : 0;
    ShopMenu shop_menu(item_service,view_state);
    if (isLoggedIn) {
        // TODO: cart interface
        shop_menu.addItem("Cart item",
            []() {
                std::cout << "Under development" << std::endl;
            });
    }
    else {
        shop_menu.addItem("Cart item",
          []() {
              std::cout << "Login first" << std::endl;
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            });
    }
    shop_menu.addItem("Next page",
        [&shop_menu]() {
            shop_menu.nextPage();
        });
    shop_menu.addItem("Previous page",
        [&shop_menu]() {
            shop_menu.prevPage();
        });
    shop_menu.addItem("All items",
        [&shop_menu, isLoggedIn]() {
            shop_menu.showAllItems(isLoggedIn);
        });
    shop_menu.addItem("Search name",
        [&shop_menu]() {
            shop_menu.searchByName();
        });
    shop_menu.addItem("Search Category",
        [&shop_menu]() {
            shop_menu.searchByCategory();
        });
    shop_menu.addItem("Search price",
        [&shop_menu]() {
            shop_menu.searchByPrice();
        });
    shop_menu.run();
}

void MainMenu::showItemManagement() const {
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
}

void MainMenu::setAppState(bool isLoggedIn, const Account &account) {
    this->appState.isLoggedIn = isLoggedIn;
    this->appState.account = account;
    this->appState.isAdmin = account.getUsername() == "root";
}


void MainMenu::updateMainMenu() {
    this->clearItem();
    // login options
    if(this->appState.isLoggedIn) {
        // admin options : items management
        if (this->appState.isAdmin) {
            this->addItem("Items Management",
                [this]() {
                    this->showItemManagement();
                });
        }
        // user options : shopping, change password
        else {
            this->addItem("Shopping",
                [this](){
                    this->showItemService(true);
                });
            this->addItem("Change Password",
            [this](){ logio.changePassword(); });
        }

        // logout
        this->addItem("Logout",
            [this]() {
                logio.logout();
                this->setAppState(false,logio.current_account());
                this->updateMainMenu();
                this->showAppState();
            });
    }
    // not login options
    else {
        // browse items
        this->addItem("Browsing",
            [this](){
                this->showItemService(false);
        });
        // login
        this->addItem("Login",
            [this]() {
                if (logio.login()) {
                    this->setAppState(true,logio.current_account());
                    this->updateMainMenu();
                    this->showAppState();
                }
            });
    }
}


