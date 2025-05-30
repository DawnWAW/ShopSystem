//
// Created by dawn on 2025/4/8.
//

#include "MainMenu.h"

#include <ItemService.h>
#include <ShopMenu.h>

#include "OrderList.h"

/// 主菜单构造函数
/// @param database 数据库依赖
/// @param logio 登陆服务类
/// @param item_service 物品服务类
/// @brief 给主菜单注入相关依赖
MainMenu::MainMenu(Database &database,Logio &logio, ItemService &item_service) : Menu("MainMenu"), database(database), logio(logio),item_service(item_service) {
    this->updateMainMenu();
}

/// 输出当前登陆的用户状态
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

/// 展示子菜单:浏览/购物
/// @param app_state 应用的登陆状态
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

/// 展示子菜单: 管理员的物品管理菜单
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

/// 设置当前主菜单的登陆状态
/// @param isLoggedIn 登陆状态
/// @param account 登陆的账号
void MainMenu::setAppState(const bool isLoggedIn, const Account &account) {
    this->appState.isLoggedIn = isLoggedIn;
    this->appState.account = account;
    this->appState.isAdmin = account.getUsername() == "root";
}

/// 展示子菜单: 订单管理
/// @param account
void MainMenu::showOrderManagement(Account &account) const {
    OrderList orders(item_service,account);

    Menu order_management_menu("Orders Management");
    if (account.getUsername() == "root") {
        order_management_menu.addItem("set order status",
            [&orders]() {
                orders.showOrderList();
                orders.setOrderStatus();
            });
    }
    else {
        order_management_menu.addItem("cancel order",
            [&orders]() {
                orders.showOrderList();
                orders.cancelOrder();
            });
        order_management_menu.addItem("update order address",
            [&orders]() {
                orders.showOrderList();
                orders.setOrderAddress();
            });
        order_management_menu.addItem("delete order",
            [&orders]() {
                orders.showOrderList();
                orders.deleteOrder();
            });
    }
    order_management_menu.run([&orders, this, account]() {
        orders.refreshList();
        orders.showOrderList();
    });

}

/// 展示折扣管理子菜单
void MainMenu::showDiscountManagement() const {
    Menu discount_management_menu("Discounts Management");

    discount_management_menu.addItem("set discount",
        [this]() {
            this->item_service.setDiscount();
        });
    discount_management_menu.addItem("delete discount",
        [this]() {
            this->item_service.deleteDiscount();
        });
    discount_management_menu.run([this]() {
        this->item_service.getAllDiscountInfos();
    });
}

/// 更新主菜单
/// @details 根据主菜单的app_state来提供不同的菜单项
void MainMenu::updateMainMenu() {
    // 1. 清空菜单项, 以便重新添加
    this->clearItem();

    // 2. loggedIn options
    if(this->appState.isLoggedIn) {
        // admin options : items, orders, discounts management
        if (this->appState.isAdmin) {
            this->addItem("Items Management",
                [this]() {
                    this->showItemManagement();
                });
            this->addItem("Orders Management",
                [this]() {
                    this->showOrderManagement(appState.account);
                });
            this->addItem("Discount Management",
                [this]() {
                    this->showDiscountManagement();
                });
        }
        // user options : shopping, orders management, change password
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
                // 调用登陆服务的登出接口, 设置当前应用状态, 更新主菜单为未登录状态
                logio.logout();
                this->setAppState(false,logio.current_account());
                this->updateMainMenu();
            });
    }
    // not loggedIn options
    else {
        // browse items
        this->addItem("Browsing",
            [this](){
                this->showItemService(appState);
        });
        // login interface
        this->addItem("Login",
            [this]() {
                if (logio.login()) {
                    // 调用登陆服务的登出接口, 设置当前应用状态, 更新主菜单为已登录状态
                    this->setAppState(true,logio.current_account());
                    this->updateMainMenu();
                    this->showAppState();
                    FormMenu::noticeTheEnter("");
                }
            });
    }
}


