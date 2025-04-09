//
// Created by dawn on 2025/4/9.
//

#ifndef LOGIO_H
#define LOGIO_H
#include "Database.h"
#include "Account.h"
#include <iostream>


class Logio {
public:
    explicit Logio(Database &database);

    [[nodiscard]] bool login();
    void logout();
    void changePassword() const;

    [[nodiscard]] Account current_account() const;

    void set_current_account(const Account &current_account);

private:
    Database &database;
    Account currentAccount;
};



#endif //LOGIO_H
