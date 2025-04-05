//
// Created by dawn on 2025/4/2.
//

#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>


class Account {
    int id;
    std::string username;
    std::string password;

public:
    Account();

    Account(const std::string &username, const std::string &password);

    Account(int id, const std::string &username, const std::string &password);

    [[nodiscard]] int getId() const;
    [[nodiscard]] std::string getUsername() const;
    [[nodiscard]] std::string getPassword() const;
};



#endif //ACCOUNT_H
