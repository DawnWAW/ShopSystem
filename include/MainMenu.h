//
// Created by dawn on 2025/4/8.
//

#ifndef MAINMENU_H
#define MAINMENU_H
#include "Menu.h"
#include "Database.h"
#include "Account.h"
#include <iostream>
#include <limits>
#include <Logio.h>
#include "ItemService.h"


class MainMenu final : public Menu {
public:
    struct AppState{
    bool isLoggedIn = false;
    bool isAdmin = false;
    Account account;
    };
    MainMenu(Database &database, Logio &logio, ItemService &item_service);

    void updateMainMenu();
    void showAppState() const;
    void showItemService(const AppState &app_state) const;
    void showItemManagement() const;
    void setAppState(bool isLoggedIn, const Account &account);
    void showOrderManagement(Account &account) const;

private:
    Database& database;
    Logio& logio;
    ItemService &item_service;
    AppState appState;
};



#endif //MAINMENU_H
