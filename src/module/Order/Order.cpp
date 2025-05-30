//
// Created by dawn on 2025/5/7.
//

#include "Order.h"

Order::Order(const Account &account): order_id(0), order_total_price(0), order_state(OrderState::PREPARING), buyer_id(account.getId()) {
    this->set_buyer_name(account.getUsername());
}

Order::Order(const int order_id,const double total_price,const int order_state, const int buyer_id, const std::string &buyer_name,const std::string &address,const int64_t order_datetime
             ): order_id(order_id),  order_total_price(total_price), buyer_id(buyer_id) {
    this->set_buyer_name(buyer_name);
    this->set_address(address);
    this->set_order_datetime(order_datetime);
    this->set_order_state(order_state);
}

int Order::get_order_id() const {
    return order_id;
}

void Order::set_order_id(const int order_id) {
    this->order_id = order_id;
}

std::string Order::get_order_datetime() const {
    return order_datetime;
}

void Order::set_order_datetime(const int64_t datetime) {
    const time_t order_time_in_seconds = datetime;
    const struct tm *local_time = localtime(&datetime);
    char buffer[80];
    strftime(buffer,sizeof(buffer),"%Y-%m-%d %H:%M:%S",local_time);
    this->order_datetime = buffer;
}

double Order::get_order_total_price() const {
    return order_total_price;
}

/// 计算订单总价格
void Order::set_order_total_price() {
    // init total price
    double total_price = 0;
    // calculate all items prices
    for (const auto &item : order_items) {
        if (item.discount.reach != 0 && item.itemPrice * item.quantity > item.discount.reach) {
            total_price += item.quantity*item.itemPrice - item.discount.cut;
        }
        else
            total_price += item.itemPrice * item.quantity;
    }
    order_total_price = total_price;
}

Order::OrderState Order::get_order_state() const {
    return order_state;
}

void Order::set_order_state(const int order_state) {
    switch (order_state) {
        case 1:
            this->order_state = OrderState::PREPARING; break;
        case 2:
            this->order_state = OrderState::TRANSPORTING; break;
        case 3:
            this->order_state = OrderState::DELIVERED; break;
        case 4:
            this->order_state = OrderState::INVISIBLE; break;
        case 5:
            this->order_state = OrderState::CANCELLED; break;
        default:
            break;
    }
}

std::string Order::order_state_toString(const OrderState order_state) {
    switch (order_state) {
        case OrderState::PREPARING:
            return "PREPARING";
        case OrderState::TRANSPORTING:
            return "TRANSPORTING";
        case OrderState::DELIVERED:
            return "DELIVERED";
        case OrderState::INVISIBLE:
            return "INVISIBLE";
        case OrderState::CANCELLED:
            return "CANCELLED";
        default:
            return "UNKNOWN";
    }
}

int Order::get_buyer_id() const {
    return buyer_id;
}

void Order::set_buyer_id(const int buyer_id) {
    this->buyer_id = buyer_id;
}

std::string Order::get_buyer_name() const {
    return buyer_name;
}

void Order::set_buyer_name(const std::string &buyer_name) {
    this->buyer_name = buyer_name;
}

std::string Order::get_address() const {
    return address;
}

void Order::set_address(const std::string &address) {
    this->address = address;
}

std::vector<Cart::SomeItems> Order::get_order_items() const {
    return order_items;
}

void Order::push_item(const Cart::SomeItems& item) {
    this->order_items.push_back(item);
}

void Order::showOrder() const {
    std::cout << "OrderID: " <<this->order_id << std::endl
            << "Datetime: " << this->order_datetime << std::endl
            << "Order State: " << order_state_toString(this->order_state) << std::endl
            << "Buyer: " << buyer_name << std::endl
            << "Address: " << address << std::endl
            << "Items:\t" << "Total price:"<< order_total_price <<std::endl
            << "**********************************" << std::endl;
    int index = 1;
    for (const auto &item : this->order_items) {
        std::cout << "   \t name\t price\t *quantity" << std::endl;
        std::cout << "[" << index++ << "] \t" <<item.itemName << "\t\t" <<item.itemPrice<<"\t * "<<item.quantity ;
        std::cout << std::endl;
    }
    std::cout << "**********************************" << std::endl;
}
