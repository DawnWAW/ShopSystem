//
// Created by dawn on 2025/4/8.
//

#include "MainMenu.h"

#include <ItemService.h>
#include <ShopMenu.h>

#include "OrderList.h"


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

void MainMenu::showItemService(const AppState &app_state) const {
    ShopMenu shop_menu(item_service,app_state);
    shop_menu.addItem("Previous page",
        [&shop_menu]() {
            shop_menu.prevPage();
        });
    shop_menu.addItem("Next page",
        [&shop_menu]() {
            shop_menu.nextPage();
        });
    shop_menu.addItem("All items",
        [&shop_menu, app_state]() {
            shop_menu.showAllItems(app_state.isLoggedIn);
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
    if (app_state.isLoggedIn) {
        shop_menu.addItem("Add to cart",
            [&shop_menu]() {
                shop_menu.cartItem();
            });
        shop_menu.addItem("Buy now",
            [&shop_menu]() {
               //TODO:BUY INTERFACE
                    shop_menu.buyMenu();
            });
        shop_menu.addItem("My Cart",
             [&shop_menu]() {
                shop_menu.showCartMenu();
            });
    }
    else {
        shop_menu.addItem("Add to cart(Login needed)",
            []() {
                FormMenu::noticeTheEnter("You need to login at first to add items to cart");
            });
        shop_menu.addItem("Buy now(Login needed)",
            []() {
                FormMenu::noticeTheEnter("You need to login at first to buy items");
            });
        shop_menu.addItem("My Cart(Login needed)",
             []() {
                 FormMenu::noticeTheEnter("You need to login at first to view the cart");
            });
    }
    shop_menu.run();

    if (app_state.isLoggedIn) {
        // update cart to database
        shop_menu.updateCart();
    }
}

void MainMenu::showItemManagement() const {
    Menu item_management_menu("Items Management");
    item_management_menu.addItem("Show all items",
                                 [this]() {
                                     item_service.showAllItems(true);
                                     FormMenu::noticeTheEnter("");
                                 });
    item_management_menu.addItem("Add item",
                                 [this]() {
                                     item_service.addItem();
                                     FormMenu::noticeTheEnter("");
                                 });
    item_management_menu.addItem("Update item",
                                 [this]() {
                                     item_service.updateItem();
                                 });
    item_management_menu.addItem("Delete item",
                                [this]() {
                                    item_service.deleteItem();
                                    FormMenu::noticeTheEnter("");
        });
    item_management_menu.run();
}

void MainMenu::setAppState(const bool isLoggedIn, const Account &account) {
    this->appState.isLoggedIn = isLoggedIn;
    this->appState.account = account;
    this->appState.isAdmin = account.getUsername() == "root";
}

// todo
void MainMenu::showOrderManagement(Account &account) const {
    OrderList orders(item_service,account);

    Menu order_management_menu("Orders Management");
    if (account.getUsername() == "root") {
        order_management_menu.addItem("set order status",
            [&orders]() {
                orders.setOrderStatus();
            });
    }
    else {
        order_management_menu.addItem("cancel order",
            [&orders]() {
                orders.cancelOrder();
            });
        order_management_menu.addItem("update order address",
            [&orders]() {
                orders.setOrderAddress();
            });
        order_management_menu.addItem("delete order",
            [&orders]() {
                orders.deleteOrder();
            });
    }
    order_management_menu.run([&orders, this, account]() {
        orders.refreshList();
        orders.showOrderList();
    });

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
            this->addItem("Orders Management",
                [this]() {
                    this->showOrderManagement(appState.account);
                });
        }
        // user options : shopping, change password
        else {
            this->addItem("Shopping",
                [this](){
                    this->showItemService(appState);
                });
            this->addItem("Orders Management",
                [this]() {
                    this->showOrderManagement(appState.account);
                });
            this->addItem("Change Password",
                [this]() {
                    logio.changePassword();
                    FormMenu::noticeTheEnter("");
            });
        }

        // logout
        this->addItem("Logout("+appState.account.getUsername()+")",
            [this]() {
                logio.logout();
                this->setAppState(false,logio.current_account());
                this->updateMainMenu();
            });
    }
    // not login options
    else {
        // browse items
        this->addItem("Browsing",
            [this](){
                this->showItemService(appState);
        });
        // login
        this->addItem("Login",
            [this]() {
                if (logio.login()) {
                    this->setAppState(true,logio.current_account());
                    this->updateMainMenu();
                    this->showAppState();
                    FormMenu::noticeTheEnter("");
                }
            });
    }
}


