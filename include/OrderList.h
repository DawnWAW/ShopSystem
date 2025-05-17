//
// Created by dawn on 2025/5/17.
//

#ifndef ORDERLIST_H
#define ORDERLIST_H
#include <ItemService.h>
#include <vector>
#include "Order.h"


class OrderList {
    std::vector<Order> orders;
    ItemService &itemService;
    Account &account;
public:
    OrderList(ItemService &itemService,Account &account);
    int getOrderByIndex() const;

    void refreshList();
    void showOrderList() const;
    void cancelOrder() const;
    void deleteOrder() const;
    void setOrderStatus() const;
    void setOrderAddress() const;
};



#endif //ORDERLIST_H
