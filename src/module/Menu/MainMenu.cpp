//
// Created by dawn on 2025/4/8.
//

#include "MainMenu.h"


MainMenu::MainMenu(Database &database,Logio &logio) : Menu("MainMenu"), database(database), logio(logio) {
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
            [this](){ std::cout<<"Under developing"<<std::endl; });
        this->addItem("Account Management",
            [this](){std::cout<<"Under developing"<<std::endl;});
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
            [this](){std::cout<<"Under developing"<<std::endl;});
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


