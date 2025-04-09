//
// Created by dawn on 2025/4/2.
//

#ifndef DATABASE_H
#define DATABASE_H
#include <optional>
#include <sqlite3.h>
#include <string>
#include <vector>
#include "Account.h"


class Database {
private:
    sqlite3 *db;
public:

    explicit Database(const std::string &dbname);

    ~Database();

    Database& openDB(const std::string &dbname);

    Database& init_table() ;

    Database& execute(const std::string &sql);

    [[nodiscard]] std::vector<std::vector<std::string>> query(const std::string& sql) const;

    [[nodiscard]] bool userExists(const std::string &username) const;

    [[nodiscard]] Account getAccount(const std::string &username) const;

    int64_t insertAccount(const Account &account) const;

    bool changePassword(const std::string &username, const std::string &newPassword) const;

    [[nodiscard]] sqlite3 *getDB() const;

};



#endif //DATABASE_H
