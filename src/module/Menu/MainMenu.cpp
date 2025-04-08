//
// Created by dawn on 2025/4/8.
//

#include "../../../include/MainMenu.h"
MainMenu::MainMenu(Database &database) :database(database){}

// void MainMenu::showAppState() const {
//     (this->appState.isLoggedIn ?
//         std::cout <<"["<<this->appState.account->getUsername()<<"](LoggedIn)"
//         : std::cout << "[Guest](Not LoggedIn)") << std::endl;
// }

void MainMenu::showMainMenu(){
    Menu mainMenu("Main Menu");
    updateMainMenu(mainMenu);
    mainMenu.run();
}

void MainMenu::updateMainMenu(Menu &menu) {
    menu.clearItem();
    if(this->appState.isLoggedIn) {
        menu.addItem("Shopping",
            [this](){ std::cout<<"Under developing"<<std::endl; });
        menu.addItem("Accout Managerment",
            [this](){std::cout<<"Under developing"<<std::endl;});
        menu.addItem("Logout",
            [this]() {
                this->appState.isLoggedIn=false;
                this->appState.isAdmin=false;
                this->appState.account=nullptr;
            });
    }
    else {
        menu.addItem("Browsing",
            [this](){std::cout<<"Under developing"<<std::endl;});
        menu.addItem("Login",
            [this]() {
                // TODO:login interface and set appstate
                this->appState.isLoggedIn=true;
            });
    }
    menu.addItem("Quit",[](){ exit(0); });
}


