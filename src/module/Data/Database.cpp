//
// Created by dawn on 2025/4/2.
//

#include "Database.h"

#include <iostream>

Database::Database(const std::string& dbname) {
    this->openDB(dbname).init_table()
        .insertAccount(Account("root","root"));
}

Database::~Database() {
    sqlite3_close(this->db);
}

Database& Database::openDB(const std::string &dbname) {
    const int rc = sqlite3_open(dbname.c_str(), &this->db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database"<< sqlite3_errmsg(this->db) << std::endl;
        sqlite3_close(db);
    }
    return *this;
}

Database& Database::init_table() {
    // create tb_account
    this->execute("create table if not exists account("
                "id integer primary key autoincrement,"
                "username varchar(20) unique not null,"
                "password varchar(20) not null);"
    )
    // create tb_items
    .execute("create table if not exists items("
            "id  INTEGER primary key autoincrement,"
            "name         TEXT    not null,"
            "category     TEXT    not null,"
            "description  TEXT,"
            "price        REAL    not null,"
            "stock        INTEGER not null,"
            "state        INTEGER not null,"
            "created_time INTEGER not null,"
            "updated_time INTEGER not null,"
            "check (price >= 0),"
            "check (stock >= 0));"
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

std::vector<int> Database::queryAllId(const std::string &table) const {
    const std::string sql = "SELECT id FROM "+table+" ORDER BY id;";
    sqlite3_stmt* stmt = nullptr;
    std::vector<int> results;

    if (sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL error: " + std::string(sqlite3_errmsg(this->db)));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        results.push_back(id);
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

    Account account;
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

bool Database::changePassword(const std::string& username, const std::string& newPassword) const {
    const char* sql = "UPDATE account SET password = ? WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL error: " + std::string(sqlite3_errmsg(this->db)));
    }

    sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}

bool Database::addItem(const Item &item) const {
    const char* sql =
        "INSERT INTO items (name, category, description, price, stock, state, created_time, updated_time) "
        "VALUES (?, ?, ?, ?, ?, ?, strftime('%s', 'now'), strftime('%s', 'now'))";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    int paramIndex = 1;

    sqlite3_bind_text(stmt, paramIndex++, item.get_name().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, paramIndex++, Item::category_to_string(item.get_category()).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, paramIndex++, item.get_description().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, paramIndex++, item.get_price());
    sqlite3_bind_int(stmt, paramIndex++, item.get_stock());
    sqlite3_bind_int(stmt, paramIndex, item.get_state());

    const bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success;
}

bool Database::updateItem(const Item &newItem) const {
    const char* sql =
    "UPDATE items SET "
    "   updated_time = strftime('%s', 'now'), "
    "   name = ? , "
    "   price = ? , "
    "   stock = ? ,"
    "   state = ? ,"
    "   category = ? ,"
    "   description = ? "
    "WHERE id = ? ; ";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    int paramIndex = 1;

    sqlite3_bind_text(stmt, paramIndex++, newItem.get_name().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, paramIndex++, newItem.get_price());
    sqlite3_bind_int(stmt, paramIndex++, newItem.get_stock());
    sqlite3_bind_int(stmt, paramIndex++, newItem.get_state());
    sqlite3_bind_text(stmt, paramIndex++, Item::category_to_string(newItem.get_category()).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, paramIndex++, newItem.get_description().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, paramIndex, newItem.get_id());

    const bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success;
}

bool Database::deleteItem(int id) const {
    const char* sql = "DELETE FROM items WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);
    const bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

std::unique_ptr<Item> Database::getItemById(const int &id) const {
    const char* sql = "SELECT * FROM items WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return nullptr;
    }

    auto item = std::make_unique<Item>(
        sqlite3_column_int(stmt, 0),      // id
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)), // name
        sqlite3_column_double(stmt, 4),   // price
        sqlite3_column_int(stmt, 5),      // stock
        sqlite3_column_int(stmt, 6),
        Item::string_to_category(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))), // category
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)), // description
        sqlite3_column_int64(stmt, 7),    // created_time
        sqlite3_column_int64(stmt, 8)     // updated_time
    );

    sqlite3_finalize(stmt);
    return item;
}

sqlite3 * Database::getDB() const {
    return this->db;
}


