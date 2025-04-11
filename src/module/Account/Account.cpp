//
// Created by dawn on 2025/4/2.
//

#include "Account.h"

#include <FormMenu.h>
#include <iostream>

Account::Account(){
    this->id = -1;
}

Account::Account(const std::string &username) :username(username){
    this->id = -1;
}

Account::Account(const std::string &username, const std::string &password)
: username(username), password(password) {
    this->id = 0;
}

Account::Account(const int id, const std::string &username, const std::string &password)
    : id(id),
      username(username),
      password(password) {
}

[[nodiscard]] int Account::getId() const {
    return id;
}

[[nodiscard]] std::string Account::getUsername() const {
    return username;
}

[[nodiscard]] std::string Account::getPassword() const {
    return password;
}

void Account::set_id(int id) {
    this->id = id;
}

void Account::set_username(const std::string &username) {
    this->username = username;
}

void Account::set_password(const std::string &password) {
    this->password = password;
}

std::string Account::input_username(const std::string &prompt) {
    std::string username;
    do {
        username = FormMenu::getStrInput(prompt);
        if (username.empty()) {
            std::cout << "Username can not be empty";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }while (username.empty());
    return username;
}


std::string Account::toString() const {
    return "Username: " + username + "\nPassword: " + password;
}
