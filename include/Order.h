//
// Created by dawn on 2025/5/7.
//

#ifndef ORDER_H
#define ORDER_H
#include <string>
#include <ctime>
#include "Cart.h"
#include <vector>
#include "Account.h"

class Order {
public:
    enum class OrderState{
        PREPARING = 1, // 准备发货
        TRANSPORTING = 2, // 运输中
        DELIVERED = 3, // 已送达
        INVISIBLE = 4,  // 不可见
        CANCELLED = 5,  // 已取消
};

private:
    int order_id;
    std::string order_datetime;
    std::vector<Cart::SomeItems> order_items;
    double order_total_price;
    OrderState order_state;
    int buyer_id;
    std::string buyer_name;
    std::string address;

public:
    [[nodiscard]] int get_order_id() const;

    void set_order_id(const int order_id);

    [[nodiscard]] std::string get_order_datetime() const;

    void set_order_datetime(int64_t datetime);

    [[nodiscard]] double get_order_total_price() const;

    void set_order_total_price();

    [[nodiscard]] OrderState get_order_state() const;

    void set_order_state(const int order_state);

    static std::string order_state_toString(OrderState order_state);

    [[nodiscard]] int get_buyer_id() const;

    void set_buyer_id(const int buyer_id);

    [[nodiscard]] std::string get_buyer_name() const;

    void set_buyer_name(const std::string &buyer_name);

    [[nodiscard]] std::string get_address() const;

    void set_address(const std::string &address);

    [[nodiscard]] std::vector<Cart::SomeItems> get_order_items() const;

    void push_item(const Cart::SomeItems& item);

    void showOrder() const;

    // generate order through "buy now"
    Order(const Account &account);

    // generate order through database
    Order(const int order_id,const double total_price,const int order_state, const int buyer_id, const std::string &buyer_name,const std::string &address,const int64_t order_datetime);
};



#endif //ORDER_H
