#include <iostream>

#include "include/Database.h"

int main() {
    // Database test
    try {
        Database database("shop.db");

        int64_t id = database.insertAccount(Account("ABD","ABD"));
        std::cout << "插入记录:" << id << std::endl;

        if (database.userExists("root")) {
            std::cout<< "user [root] exists" << std::endl;
        }
        else {
            std::cout << "user [root] does not exist" << std::endl;
        }

        if (database.userExists("ABD")) {
            std::cout<< "user [root] exists" << std::endl;
            Account account = database.getAccount("ABD");
            std::cout<< "userid   [" << account.getId() << "]" << std::endl
                     << "username [" << account.getUsername() << "]" << std::endl
                     << "userpswd [" << account.getPassword() << "]" << std::endl;
        }
        else {
            std::cout << "user [ABD] does not exist" << std::endl;
        }

        auto result = database.query("select * from account");
        for (const auto& row : result) {
            std::cout <<  "ID: "<< row[0] << " Username: " << row[1] << " Password: " << row[2] << std::endl;
        }
    }catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
