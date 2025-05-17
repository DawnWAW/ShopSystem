//
// Created by dawn on 2025/5/17.
//

#include "OrderList.h"

OrderList::OrderList(ItemService &itemService, Account &account):itemService(itemService),account(account) {
    if (account.getUsername() == "root") {
        this->orders = itemService.queryAllOrders();
    }
    else
        this->orders = itemService.queryOrderByAccount(account);
}

int OrderList::getOrderByIndex() const {
    int index = 0;
    do {
        if (index < 0 || index >= this->orders.size()) {
            std::cout<< "invalid index" << std::endl;
        }
        index = FormMenu::getIntInput("Input order index:")-1;
    }while ( index < 0 || index >= this->orders.size() );
    return index;
}

void OrderList::refreshList() {
    this->orders.clear();
    if (account.getUsername() == "root") {
        this->orders = itemService.queryAllOrders();
    }
    else
        this->orders = itemService.queryOrderByAccount(account);
}

void OrderList::showOrderList() const {
    int index = 1;
    for (const auto order : orders) {
        if (this->account.getUsername()!="root") {
            if (order.get_order_state() == Order::OrderState::INVISIBLE){
                index++;
                continue;
            }
        }
        std::cout << "Order Index: " << index++ << std::endl;
        order.showOrder();
    }
}

void OrderList::cancelOrder() const {
    Order order = orders[this->getOrderByIndex()];

    if (order.get_order_state()!=Order::OrderState::PREPARING) {
        FormMenu::noticeTheEnter("Order is not in preparing, can not be canceled");
        return;
    }
    order.showOrder();

    if (itemService.setOrderStatus(order,5)) {
        for (const auto &item : order.get_order_items()) {
            if (!itemService.updateItemStock(item.itemId,itemService.getItemStock(item.itemId)+item.quantity)) {
                throw std::runtime_error("update order failed");
            }
        }
        FormMenu::noticeTheEnter("Cancel order successfully");
    }
    else {
        throw std::runtime_error("cancel order failed");
    }
}

void OrderList::deleteOrder() const {
    Order order = orders[this->getOrderByIndex()];

    if (!(order.get_order_state()==Order::OrderState::DELIVERED||order.get_order_state()==Order::OrderState::CANCELLED)) {
        FormMenu::noticeTheEnter("Order is not delivered yet, can not be deleted");
        return;
    }
    order.showOrder();
    if (itemService.setOrderStatus(order,4)) {
        FormMenu::noticeTheEnter("Delete order successfully");
    }
    else {
        FormMenu::noticeTheEnter("delete order failed");
    }
}

void OrderList::setOrderStatus() const {
    Order order = orders[this->getOrderByIndex()];

    FormMenu select_status("change the order status to");
    select_status.addItem("Preparing",[&order,this]() {
        itemService.setOrderStatus(order,1);
    });
    select_status.addItem("Transporting",[&order,this]() {
        itemService.setOrderStatus(order,2);
    });
    select_status.addItem("Delivered",[&order,this]() {
        itemService.setOrderStatus(order,3);
    });
    select_status.addItem("Canceled",[&order,this]() {
        itemService.setOrderStatus(order,5);
    });
    select_status.addItem("INVISIBLE",[&order,this]() {
        itemService.setOrderStatus(order,4);
    });
    order.showOrder();
    select_status.run();
}

void OrderList::setOrderAddress() const {
    Order order = orders[this->getOrderByIndex()];

    if (order.get_order_state()!=Order::OrderState::PREPARING) {
        FormMenu::noticeTheEnter("Order is transporting, can not update address");
        return;
    }

    if (itemService.setOrderAddress(order,FormMenu::getStrInput("Enter new address:"))) {
        FormMenu::noticeTheEnter("update address successfully");
    }
    else {
        FormMenu::noticeTheEnter("update address failed");
    }
}




