//
// Created by dawn on 2025/5/8.
//
#include <functional>

#include "Database.h"
#include "ItemService.h"

/*int main() {
    Database database("shop.db");

    // 1. test if order can be successfully store in db
    Order newOrder(4,299.2,1,2,"333","666",123);
    newOrder.push_item({1,"23",231.9,12});
    if (database.addOrder(newOrder)) {
        std::cout << "yes" << std::endl;
    }

    // 2. test if order can be read
    database.getOrderById(1)->showOrder();

}*/

// test auto status switch
void func() {
    Database database("shop.db");
    ItemService item_service(database);
    std::unique_ptr<Order> order = database.getOrderById(1);
    std::cout << "order status before auto switch is " << Order::order_state_toString(order->get_order_state()) << std::endl;
    item_service.autoStatuSwitch(1,std::chrono::seconds(5));
    // order->set_order_state(3);
    // if(database.updateOrder(*order)) {
    //     std::cout << "order status when auto switch changed to " << Order::order_state_toString(order->get_order_state()) << std::endl;
    // }
    order = database.getOrderById(1);
    std::cout << "order status after auto switch is " << Order::order_state_toString(order->get_order_state()) << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(7));

}
int main() {
    func();
}
