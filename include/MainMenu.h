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


class MainMenu final : public Menu {
public:
  struct AppState{
    bool isLoggedIn = false;
    bool isAdmin = false;
    Account account;
  };
  MainMenu(Database &database, Logio &logio);

  void updateMainMenu();

  void showAppState() const;
private:
    Database& database;
    Logio& logio;
    AppState appState;
};



#endif //MAINMENU_H
