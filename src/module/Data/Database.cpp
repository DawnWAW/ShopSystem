//
// Created by dawn on 2025/4/2.
//

#include "Database.h"

#include <iostream>

/// 数据库构造函数
/// @param dbname 数据库名称
Database::Database(const std::string& dbname) {
    // 打开数据库
    this->openDB(dbname)
    .init_table()// 初始化表格
    .insertAccount(Account("root","root"));// 插入管理员数据信息
}

Database::~Database() {
    sqlite3_close(this->db);
}

/// 打开数据库
/// @param dbname 数据库文件名称
/// @return 打开的数据库
Database& Database::openDB(const std::string &dbname) {
    const int rc = sqlite3_open(dbname.c_str(), &this->db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database"<< sqlite3_errmsg(this->db) << std::endl;
        sqlite3_close(db);
    }
    return *this;
}

/// 初始化数据库表格, 对于没有的表进行插入
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
    )
    // create tb_cart
    .execute("create table if not exists carts("
            "id  INTEGER primary key autoincrement,"
            "item_id      INTEGER    not null,"
            "account_id   INTEGER    not null,"
            "quantity     INTEGER    not null,"
            "check (quantity >= 0));"
    )
    //create user-orders table
    .execute(
        "create table if not exists user_orders("
        "id    integer primary key autoincrement,"
        "total_price real              not null,"
        "order_state integer default 1 not null,"
        "account_id  integer           not null,"
        "buyer_name  varchar(20)       not null,"
        "address     varchar(100)      not null,"
        "order_time  integer           not null);"
    )
    // create order-items table
    .execute(
        "create table if not exists order_items("
        "id            integer   primary key autoincrement,"
        "order_id      integer not null,"
        "item_id       integer not null,"
        "item_name     text    not null,"
        "item_quantity integer not null,"
        "item_price    real    not null);"
    )
    //discount table
    .execute("create table if not exists item_discount("
        "id         integer primary key,"
        "discount   integer default 0,"
        "reach      real    default 0,"
        "cut        real    default 0,"
        "start_time integer not null,"
        "end_time   integer not null);"
    );
    return *this;
}

/// 对数据库执行sql语句
/// @param sql 要执行的数据库语句
/// @return 数据库引用
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

/// 获取表中所有数据的ID
/// @param table 要获取所有信息的表名称
/// @return ID列表
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

/// 查询用户是否存在
/// @param username 查询的用户名
/// @return 是否存在
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

/// 获取用户信息
/// @param username 查询的用户
/// @return 用户信息
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

/// 插入用户到数据中
/// @param account 要添加的用户信息
/// @return 用户在表中的ID
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

/// 更改用户密码
/// @param username 用户名
/// @param newPassword 用户新密码
/// @return 更改是否成功
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

/// 添加物品
/// @param item 待添加的物品
/// @return 结果
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
    sqlite3_bind_text(stmt, paramIndex++, item.get_category().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, paramIndex++, item.get_description().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, paramIndex++, item.get_price());
    sqlite3_bind_int(stmt, paramIndex++, item.get_stock());
    sqlite3_bind_int(stmt, paramIndex, item.get_state());

    const bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success;
}

/// 更新物品
/// @param newItem 要更新的物品
/// @return 结果
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
    sqlite3_bind_text(stmt, paramIndex++, newItem.get_category().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, paramIndex++, newItem.get_description().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, paramIndex, newItem.get_id());

    const bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success;
}

/// 删除的物品
/// @param id 对应物品的ID
/// @return 结果
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

/// 通过ID获取物品
/// @param id 物品ID
/// @return 物品指针
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
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)), // category
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)), // description
        sqlite3_column_int64(stmt, 7),    // created_time
        sqlite3_column_int64(stmt, 8)     // updated_time
    );

    item->set_discount(getDiscount(id));

    sqlite3_finalize(stmt);
    return item;
}

/// 通过名字获取物品
/// @param name 物品名
/// @return 物品ID
std::vector<int> Database::getItemByName(const std::string &name) const {
    const std::string sql = "SELECT id FROM items "
                            "WHERE name LIKE '%" + name + "%' OR description LIKE '%" + name + "%'"
                            "ORDER BY id;";
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

/// 通过种类获取物品
/// @param category 种类
/// @return 物品ID
std::vector<int> Database::getItemByCategory(const std::string &category) const {
    const std::string sql = "SELECT id FROM items "
                            "WHERE category LIKE '%" + category + "%' ORDER BY id;";
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

/// 通过价格范围获取物品
/// @param min_price 最小价格
/// @param max_price 最大价格
/// @return 符合范围的物品ID向量
std::vector<int> Database::getItemByPrice(const double &min_price, const double &max_price) const {
    const std::string sql = "SELECT id FROM items "
                        "WHERE price BETWEEN ? AND ?"
                        "ORDER BY id;";
    sqlite3_stmt* stmt = nullptr;
    std::vector<int> results;

    if (sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL error: " + std::string(sqlite3_errmsg(this->db)));
    }

    sqlite3_bind_double(stmt, 1, min_price);
    sqlite3_bind_double(stmt, 2, max_price);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        results.push_back(id);
    }

    sqlite3_finalize(stmt);
    return results;
}

/// 获取购物车物品
/// @param account_id 用户ID
/// @return 购物车中的物品
std::vector<Cart::SomeItems> Database::getCartItems(int account_id) const {
    const char* sql = "SELECT  c.item_id, i.name, c.quantity, i.price from carts c, items i where c.account_id = ? and c.item_id == i.id;";
    sqlite3_stmt* stmt = nullptr;
    std::vector<Cart::SomeItems> cart_items;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL error: " + std::string(sqlite3_errmsg(this->db)));
    }

    sqlite3_bind_int(stmt, 1, account_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Cart::SomeItems item;
        item.itemId = sqlite3_column_int(stmt, 0);
        item.itemName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
        item.quantity = sqlite3_column_int(stmt, 2);
        item.itemPrice = sqlite3_column_double(stmt, 3);
        item.discount = getDiscount(item.itemId);
        item.set_discount_price();
        cart_items.push_back(item);
    }

    sqlite3_finalize(stmt);
    return cart_items;
}

/// 删除购物车中的物品
/// @param account_id 用户ID
/// @return 结果
bool Database::deleteCartItems(int account_id) const {
    const char* sql = "DELETE FROM carts WHERE account_id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    sqlite3_bind_int(stmt, 1, account_id);
    const bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

/// 存储用户的购物车物品
/// @param cart_items 购物车的物品
/// @param account_id 用户ID
/// @return 结果
bool Database::setCartItems(const std::vector<Cart::SomeItems> &cart_items, int account_id) const {
    const char* sql =
        "INSERT INTO carts (item_id, account_id, quantity) "
        "VALUES (?, ?, ?)";

    sqlite3_stmt* stmt;

    // 开始事务
    if (sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        return false;
    }

    // 准备SQL语句
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    // 绑定并执行每条数据
    for (const auto& item : cart_items) {
        sqlite3_bind_int(stmt, 1, item.itemId);
        sqlite3_bind_int(stmt, 2, account_id);
        sqlite3_bind_int(stmt, 3, item.quantity);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }

        sqlite3_reset(stmt);  // 重置语句以便下次使用
    }

    // 清理并提交事务
    sqlite3_finalize(stmt);
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        return false;
    }

    return true;
}

/// 添加订单
/// @param order 订单
/// @return 结果
bool Database::addOrder(Order &order) const {
    const auto sql1 = "INSERT INTO user_orders "
                      "(total_price, order_state, account_id, buyer_name, address, order_time) "
                      "VALUES (?, ?, ?, ?, ?, strftime('%s', 'now'));" ;

    // prepare the sql
    sqlite3_stmt *stmt1, *stmt2;

    // 开始事务
    if (sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        return false;
    }

    // 准备SQL语句
    if (sqlite3_prepare_v2(db, sql1, -1, &stmt1, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    int paramIndex = 1;

    sqlite3_bind_double(stmt1, paramIndex++, order.get_order_total_price());
    sqlite3_bind_int(stmt1, paramIndex++, static_cast<int>(order.get_order_state()));
    sqlite3_bind_int(stmt1, paramIndex++, order.get_buyer_id());
    sqlite3_bind_text(stmt1, paramIndex++, order.get_buyer_name().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt1, paramIndex, order.get_address().c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt1) != SQLITE_DONE) {
        sqlite3_finalize(stmt1);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        std::cout << std::string(sqlite3_errmsg(this->db));
        return false;
    }

    // 查询最后插入的ID
    sqlite3_stmt* id_stmt;
    const char* id_sql = "SELECT last_insert_rowid();";
    if (sqlite3_prepare_v2(db, id_sql, -1, &id_stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(id_stmt) == SQLITE_ROW) {
            order.set_order_id(sqlite3_column_int(id_stmt, 0));
        }
        sqlite3_finalize(id_stmt);
    }

    const auto sql2 = "INSERT INTO order_items (order_id, item_id, item_name, item_quantity, item_price) "
                       "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    // add items
    for (const auto &item : order.get_order_items()) {
        sqlite3_bind_int64(stmt2, 1, order.get_order_id());
        sqlite3_bind_int(stmt2, 2, item.itemId);
        sqlite3_bind_text(stmt2, 3, item.itemName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt2, 4, item.quantity);
        sqlite3_bind_double(stmt2, 5, item.itemPrice);

        if (sqlite3_step(stmt2) != SQLITE_DONE) {
            sqlite3_finalize(stmt2);
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }
        sqlite3_reset(stmt2);  // 重置语句以便下次使用
    }
    
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);

    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        return false;
    }
    return true;
}

/// 更新订单
/// @param new_order 更新后的订单
/// @return 结果
bool Database::updateOrder(const Order &new_order) const {
    const auto sql = "UPDATE user_orders set order_state = ?, address = ? where id = ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, static_cast<int>(new_order.get_order_state()));
    sqlite3_bind_text(stmt, 2, new_order.get_address().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, new_order.get_order_id());

    const bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success;
}

/// 通过ID获得订单
/// @param id 订单ID
/// @return 订单指针
std::unique_ptr<Order> Database::getOrderById(const int &id) const {
    // search for order
    const char* sql = "SELECT * FROM user_orders WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return nullptr;
    }

    auto order = std::make_unique<Order>(
        sqlite3_column_int64(stmt, 0),
        sqlite3_column_double(stmt, 1),
        sqlite3_column_int64(stmt, 2),
        sqlite3_column_int64(stmt, 3),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)),
        sqlite3_column_int64(stmt,6)
    );
    sqlite3_finalize(stmt);

    // search for order items
    const char* sql2 = "SELECT item_id,item_name,item_price,item_quantity FROM order_items WHERE order_id = ?;";
    sqlite3_stmt* stmt2;

    if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, nullptr) != SQLITE_OK) {
        return nullptr;
    }

    sqlite3_bind_int(stmt2, 1, id);

    while (sqlite3_step(stmt2) == SQLITE_ROW) {
        order->push_item({
            sqlite3_column_int(stmt2, 0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt2, 1)),
            sqlite3_column_double(stmt2, 2),
            sqlite3_column_int(stmt2, 3),
        });
    }
    sqlite3_finalize(stmt2);
    return order;
}

/// 通过账户获得订单
/// @param account 账号
/// @return 订单ID向量
std::vector<int> Database::getOrdersByAccount(const Account &account) const {
    std::vector<int> orders;
    const auto sql = "SELECT id FROM user_orders where account_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL error: " + std::string(sqlite3_errmsg(this->db)));
    }
    sqlite3_bind_int(stmt, 1, account.getId());

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        orders.push_back(id);
    }

    sqlite3_finalize(stmt);
    return orders;

}

/// 获取折扣
/// @param id 物品的ID
/// @return 折扣信息
Item::Discount Database::getDiscount(const int id) const {
    const auto sql = "select discount,reach,cut from item_discount "
                     "where id == ? "
                     "and CAST(strftime('%s','now') AS INTEGER) between start_time and end_time;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL error: " + std::string(sqlite3_errmsg(this->db)));
    }
    sqlite3_bind_int(stmt, 1, id);

    Item::Discount discount;

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return discount;
    }

    discount.percent_off = sqlite3_column_int(stmt, 0);
    discount.reach = sqlite3_column_double(stmt, 1);
    discount.cut = sqlite3_column_double(stmt, 2);

    sqlite3_finalize(stmt);
    return discount;

}

/// 获取折扣信息(带有效时间)
/// @param id 订单ID
/// @param start_time 开始时间返回位置
/// @param end_time 结束时间返回位置
/// @return 折扣信息
Item::Discount Database::getDiscount(const int id, std::string &start_time, std::string &end_time) const {
    const auto sql = "select discount,reach,cut,datetime(start_time,'unixepoch','localtime'),datetime(end_time,'unixepoch','localtime') "
                     "from item_discount where id == ? ";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL error: " + std::string(sqlite3_errmsg(this->db)));
    }
    sqlite3_bind_int(stmt, 1, id);

    Item::Discount discount;

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return discount;
    }

    discount.percent_off = sqlite3_column_int(stmt, 0);
    discount.reach = sqlite3_column_double(stmt, 1);
    discount.cut = sqlite3_column_double(stmt, 2);
    start_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    end_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

    sqlite3_finalize(stmt);
    return discount;

}

/// 添加或更新折扣信息
/// @param id 物品ID
/// @param discount 折扣信息
/// @param start_time 开始时间
/// @param end_time 结束时间
/// @return 结果
bool Database::setDiscount(const int id, const Item::Discount &discount, const int64_t start_time,
                           const int64_t end_time) const {
    //TODO: insert or update a discount
    const auto sql = "insert or replace into item_discount "
                     "(id, discount, reach, cut, start_time, end_time) "
                     "VALUES (?,?,?,?,?,?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL error: " + std::string(sqlite3_errmsg(this->db)));
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_int(stmt, 2, discount.percent_off);
    sqlite3_bind_double(stmt, 3, discount.reach);
    sqlite3_bind_double(stmt, 4, discount.cut);
    sqlite3_bind_int64(stmt, 5, start_time);
    sqlite3_bind_int64(stmt, 6, end_time);

    const bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success;
}

/// 删除折扣
/// @param id 物品ID
/// @return 结果
bool Database::deleteDiscount(const int id) const {
    const char* sql = "DELETE FROM item_discount WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);
    const bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

sqlite3 * Database::getDB() const {
    return this->db;
}


