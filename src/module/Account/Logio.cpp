//
// Created by dawn on 2025/4/9.
//

#include "Logio.h"

Logio::Logio(Database &database):database(database) {
}

bool Logio::login() {
    Account account;
    std::cout << "=== User Login ===\n";
    // 1. get username
    account.set_username(Account::input_username("username: "));

    // 2. check if user exists
    if (database.userExists(account.getUsername())) {
        // 2.1 user exists, go to log in
        account = database.getAccount(account.getUsername());
        // get password
        for (int i = 2; i >= 0; i--) {
            if (account.getPassword() != FormMenu::getStrInput("password: ")) {
                // password wrong, input again
                if (i) {
                    std::cout << "Wrong password!" << std::endl
                     << "You still have "<< i <<" times to try." <<std::endl;
                    FormMenu::noticeTheEnter("");
                }
            }
            else {
                // password right, login success
                std::cout << "Login succeeded!" << std::endl;
                this->set_current_account(account);
                return true;
            }
        }
        // password input wrong too many times, login fail
        std::cout << "Login failed!" << std::endl;
        FormMenu::noticeTheEnter("");
        return false;
    }
    // user doesn't exist, ask if user want to register
    else {
        std::cout << "User haven't register" << std::endl;
        FormMenu ackMenu("Do you want to register User [" + account.getUsername() + "]?");
        ackMenu.addItem("Yes",
            [this, &account]() {
                // go to register
                account.set_password(FormMenu::getStrInput("Set your password: "));
                // add userdata to db
                if (database.insertAccount(account)) {
                    std::cout << "Registration succeeded!" << std::endl
                    << account.toString() << std::endl
                    << "You can now go to login" << std::endl;
                    FormMenu::noticeTheEnter("");
                }
                else {
                    std::cout << "Registration failed!" << std::endl;
                    FormMenu::noticeTheEnter("");
                }
            });
        ackMenu.addItem("No"
            ,[]() {});
        ackMenu.run();

        // do not register, return false
        return false;
    }
}

void Logio::logout() {
    this->currentAccount = Account("Guest");
}

void Logio::changePassword() const {
    std::string newPassword = FormMenu::getStrInput("Enter new password: ");

    if(database.changePassword(this->currentAccount.getUsername(),newPassword)) {
        std::cout << "Password changed succeeded"<<std::endl
        << "Your new password: "<<newPassword<<std::endl;
    }
    else {
        std::cout << "Password changed failed"<<std::endl;
    }
}

[[nodiscard]] Account Logio::current_account() const {
    return currentAccount;
}

void Logio::set_current_account(const Account &current_account) {
    currentAccount = current_account;
}
