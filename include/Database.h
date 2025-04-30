//
// Created by dawn on 2025/4/2.
//

#ifndef DATABASE_H
#define DATABASE_H
#include <sqlite3.h>
#include <string>
#include <vector>
#include "Account.h"
#include "Item.h"
#include "Cart.h"
#include <memory>


class Database {
private:
    sqlite3 *db;
public:

    explicit Database(const std::string &dbname);

    ~Database();

    Database& openDB(const std::string &dbname);

    Database& init_table() ;

    Database& execute(const std::string &sql);

    // database for account
    [[nodiscard]] bool userExists(const std::string &username) const;

    [[nodiscard]] Account getAccount(const std::string &username) const;

    int64_t insertAccount(const Account &account) const;

    [[nodiscard]] bool changePassword(const std::string &username, const std::string &newPassword) const;

    // database for item
    [[nodiscard]] std::vector<int> queryAllId(const std::string &table) const;

    [[nodiscard]] bool addItem(const Item& item) const;

    [[nodiscard]] bool updateItem(const Item &newItem) const;

    [[nodiscard]] bool deleteItem(int id) const;

    [[nodiscard]] std::unique_ptr<Item> getItemById(const int &id) const;

    [[nodiscard]] std::vector<int> getItemByName(const std::string &name) const;

    [[nodiscard]] std::vector<int> getItemByCategory(const std::string &category) const;

    [[nodiscard]] std::vector<int> getItemByPrice(const double &min_price, const double &max_price) const;

    // database for cart
    [[nodiscard]] std::vector<Cart::CartItem> getCartItems(int account_id) const;

    [[nodiscard]] bool deleteCartItems(int account_id) const;

    [[nodiscard]] bool setCartItems(const std::vector<Cart::CartItem> &cart_items, int account_id) const;



    [[nodiscard]] sqlite3 *getDB() const;

};



#endif //DATABASE_H
