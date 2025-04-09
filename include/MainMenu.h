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


class MainMenu final : public Menu {
public:
  struct AppState{
    bool isLoggedIn = false;
    bool isAdmin = false;
    Account* account = nullptr;
  };
  explicit MainMenu(Database &database);

  void updateMainMenu();

  void showAppState() const;
private:
    Database& database;
    AppState appState;
};



#endif //MAINMENU_H
