//
// Created by dawn on 2025/4/8.
//

#include "MainMenu.h"


MainMenu::MainMenu(Database &database) : Menu("MainMenu"), database(database) {
    this->updateMainMenu();
}

// void MainMenu::showAppState() const {
//     (this->appState.isLoggedIn ?
//         std::cout <<"["<<this->appState.account->getUsername()<<"](LoggedIn)"
//         : std::cout << "[Guest](Not LoggedIn)") << std::endl;
// }


void MainMenu::updateMainMenu() {
    this->clearItem();
    if(this->appState.isLoggedIn) {
        this->addItem("Shopping",
            [this](){ std::cout<<"Under developing"<<std::endl; });
        this->addItem("Accout Managerment",
            [this](){std::cout<<"Under developing"<<std::endl;});
        this->addItem("Logout",
            [this]() {
                // TODO:logout interface and set appstate
                this->appState.isLoggedIn=false;
                this->appState.isAdmin=false;
                this->appState.account=nullptr;
                updateMainMenu();
            });
    }
    else {
        this->addItem("Browsing",
            [this](){std::cout<<"Under developing"<<std::endl;});
        this->addItem("Login",
            [this]() {
                // TODO:login interface and set appstate
                this->appState.isLoggedIn=true;
                updateMainMenu();
            });
    }
}


