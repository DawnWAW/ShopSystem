//
// Created by dawn on 2025/4/2.
//

#include "../../../include/Account.h"

Account::Account(){
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