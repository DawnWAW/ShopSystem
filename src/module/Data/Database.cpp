//
// Created by dawn on 2025/4/2.
//

#include "../../../include/Database.h"

#include <iostream>

Database::Database(const std::string& dbname) {
    this->openDB(dbname).init_table()
        .insertAccount(Account("root","root"));
}

Database::~Database() {
    sqlite3_close(this->db);
    std::cout << "Database closed" << std::endl;
}

Database& Database::openDB(const std::string &dbname) {
    const int rc = sqlite3_open(dbname.c_str(), &this->db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database"<< sqlite3_errmsg(this->db) << std::endl;
        sqlite3_close(db);
    }
    else {
        std::cout << "Database opened" << std::endl;
    }
    return *this;
}

Database& Database::init_table() {
    this->execute("create table if not exists account("
                "id integer primary key autoincrement,"
                "username varchar(20) unique not null,"
                "password varchar(20) not null);"
    );
    return *this;
}

Database& Database::execute(const std::string &sql){
    char *errmsg = nullptr;
    const int rc = sqlite3_exec(this->db, sql.c_str(),nullptr,nullptr,&errmsg);
    if (rc != SQLITE_OK) {
        const std::string err = "SQL error: " + std::string(errmsg);
        sqlite3_free(errmsg);
        throw std::runtime_error(err);
    }
    return *this;
}

std::vector<std::vector<std::string>> Database::query(const std::string &sql) const {
    sqlite3_stmt *stmt = nullptr;
    std::vector<std::vector<std::string>> results;

    if (sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL error: " + std::string(sqlite3_errmsg(this->db)));
    }

    int cols = sqlite3_column_count(stmt);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<std::string> row;
        for (int i = 0; i < cols; i++) {
            const char* val = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row.emplace_back(val);
        }
        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}

bool Database::userExists(const std::string &username) const {
    sqlite3_stmt *stmt = nullptr;
    auto sql = "SELECT 1 FROM account WHERE username = ? LIMIT 1;";

    if (sqlite3_prepare_v2(this->db, sql,-1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL error: " + std::string(sqlite3_errmsg(this->db)));
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, nullptr);

    const bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exists;
}

Account Database::getAccount(const std::string &username) const {

    sqlite3_stmt *stmt = nullptr;
    auto sql = "SELECT id, username, password FROM account WHERE username = ? LIMIT 1;";

    if (sqlite3_prepare_v2(this->db, sql,-1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL error: " + std::string(sqlite3_errmsg(this->db)));
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, nullptr);

    Account account{};
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        account = Account(
            sqlite3_column_int(stmt, 0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    }

    sqlite3_finalize(stmt);
    return account;
}

int64_t Database::insertAccount(const Account &account) const {
    std::string username = account.getUsername();
    std::string password = account.getPassword();

    sqlite3_stmt *stmt = nullptr;
    const char* sql = "INSERT OR IGNORE INTO account (username, password) VALUES (?, ?);";
    const int rc = sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("SQL error: " + std::string(sqlite3_errmsg(this->db)));
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("INSERT error: " + std::string(sqlite3_errmsg(this->db)));
    }

    const int64_t id = sqlite3_last_insert_rowid(this->db);
    sqlite3_finalize(stmt);
    return id;
}

sqlite3 * Database::getDB() const {
    return this->db;
}


