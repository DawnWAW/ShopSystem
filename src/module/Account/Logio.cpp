//
// Created by dawn on 2025/4/9.
//

#include "Logio.h"

Logio::Logio(Database &database):database(database) {
}

bool Logio::login() {
    std::string username, password;
    Account account;

    std::cout << "=== User Login ===\n";
    // 1. get username
    std::cout << "username: ";
    std::getline(std::cin, username);
    account.set_username(username);

    // 2. check if user exists
    if (database.userExists(username)) {
        // 2.1 user exists, go to log in
        account = database.getAccount(username);
        // get password
        for (int i = 2; i >= 0; i--) {
            std::cout << "password: ";
            std::getline(std::cin, password);

            if (account.getPassword() != password) {
                // password wrong, input again
                if (i) {
                    std::cout << "Wrong password!" << std::endl
                     << "You still have "<< i <<" times to try." <<std::endl;
                }
            }
            else {
                // password right, login success
                std::cout << "Login successful!" << std::endl;
                this->set_current_account(account);
                return true;
            }
        }
        // password input wrong too many times, login fail
        std::cout << "Login failed!" << std::endl;
        return false;
    }
    // user doesn't exist, ask if user want to register
    else {
        std::string ack;
        std::cout << "User haven't register" << std::endl
                << "Do you want to register User[" << username << "]? (y/n)" << std::endl;
        std::getline(std::cin, ack);
        if (ack == "Y" || ack == "y") {
            // go to register
            std::cout << "Set your password:";
            std::getline(std::cin, password);

            account.set_password(password);
            // add userdata to db
            if (database.insertAccount(account)) {
                std::cout << "Registration successful!" << std::endl
                << account.toString() << std::endl
                << "You can now go to login" << std::endl;
            }
            else {
                std::cout << "Registration failed!" << std::endl;
            }
        }
        else {
            std::cout << "Register cancelled"<<std::endl;
        }
        // do not register, return
        return false;
    }
}

void Logio::logout() {
    this->currentAccount = Account("Guest");
}

void Logio::changePassword() const {
}

[[nodiscard]] Account Logio::current_account() const {
    return currentAccount;
}

void Logio::set_current_account(const Account &current_account) {
    currentAccount = current_account;
}
