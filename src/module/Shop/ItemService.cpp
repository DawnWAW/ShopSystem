//
// Created by dawn on 2025/4/10.
//

#include "ItemService.h"
ItemService::ItemService(Database &database) : database(database) {}

void ItemService::addItem() const {
    std::cout << "Adding item: " << std::endl;
    // 1. item name
    std::string itemName = Item::input_name("Enter item name: ");

    // 2. item category
    std::string category = Item::input_category("Choose item category: ");

    // 3. description
    std::string description = FormMenu::getStrInput("Enter item description (can be null): ");

    // 4. price
    double price = Item::input_price("Enter item price: ");

    // 5. stock
    int stock = Item::input_stock("Enter item stock: ");

    // 6. sale state
    int state = Item::input_state("Choose item sale state: ");


    // 7. show item info
    std::cout << "========================" << std::endl;
    std::cout << "Item Info: " <<std::endl
    << "ItemName: " << itemName << std::endl
    << "Category: " << category << std::endl
    << "Description: " << description << std::endl
    << "Price: " << price << std::endl
    << "Stock: " << stock << std::endl;
    std::cout << "State: ";
    if (state) {
        std::cout << "On sale" << std::endl;
    }else {
        std::cout << "Not on sale" << std::endl;
    }

    // 8. get ack
    FormMenu ackMenu("Are you sure to add this item?");
    ackMenu.addItem("Yes",
        [this, &itemName, &category, &description, &price, &stock, &state]() {
            Item item(itemName,price,stock,state,category,description);
            if(database.addItem(item)) {
                std::cout << "Item adding succeeded" << std::endl;
            }
            else {
                std::cout << "Item adding failed" << std::endl;
            }
        });
    ackMenu.addItem("No",
        [](){std::cout << "Item adding canceled" << std::endl;});
    ackMenu.run();
}

void ItemService::deleteItem() const {
    std::unique_ptr<Item> item = database.getItemById(FormMenu::getIntInput("Enter item id: "));
    if ( item == nullptr ) {
        std::cout << "Item not found." << std::endl;
        return;
    }

    item->display(true);
    FormMenu ackMenu("Are you sure to delete this item?");
    ackMenu.addItem("Yes",
        [this,&item]() {
           if (database.deleteItem(item->get_id())) {
               std::cout << "Item deleted succeeded" << std::endl;
           }
           else {
                std::cout << "Item deleted failed" << std::endl;
           }
        });
    ackMenu.addItem("No",
        []() {
            std::cout << "Item deleted canceled" << std::endl;
        });
    ackMenu.run();
}

void ItemService::updateItem() const {
    std::unique_ptr<Item> item = database.getItemById(FormMenu::getIntInput("Enter item id: "));
    if ( item == nullptr ){
        FormMenu::noticeTheEnter("Item not found");
        return;
    }

    Menu updateMenu("Update item options: ");
    updateMenu.addItem("Update name",
        [&item]() {
            item->set_name(Item::input_name("Enter new item name: "));
        });
    updateMenu.addItem("Update price",
                       [&item]() {
                           item->set_price(Item::input_price("Enter new item price: "));
                       });
    updateMenu.addItem("Update stock",
                       [&item]() {
                           item->set_stock(Item::input_stock("Enter new item stock: "));
                       });
    updateMenu.addItem("Update state",
                       [&item]() {
                           item->set_state(Item::input_state("Choose new item state: "));
                       });
    updateMenu.addItem("Update category",
                       [&item]() {
                            item->set_category(Item::input_category("Choose new item category: "));
                       });
    updateMenu.addItem("Update description",
                       [&item]() {
                           item->set_description(FormMenu::getStrInput("Enter new item description: "));
                       });
    updateMenu.addItem("Submit",
                        [&item, this]() {
                            if (database.updateItem(*item)) {
                                std::cout << "Item update succeeded" << std::endl;
                            }
                            else {
                                std::cout << "Item update failed" << std::endl;
                            }
                            FormMenu::noticeTheEnter("");
                        });
    updateMenu.run([&item]() {
        item->display();
    });
}

std::vector<Item> ItemService::queryAllItems() const {
    std::vector<Item> items;
    std::vector<int> ids = database.queryAllId("items");
    for (auto id : ids) {
        if (getItemStock(id)!=0) {
            items.push_back(*database.getItemById(id));
        }
    }
    return items;
}

[[nodiscard]] std::vector<Item> ItemService::queryItemsByName(const std::string &name) const {
    std::vector<Item> items;
    std::vector<int> ids = database.getItemByName(name);
    for (auto id : ids) {
        items.push_back(*database.getItemById(id));
    }
    return items;
}

[[nodiscard]] std::vector<Item> ItemService::queryItemsByCategory(const std::string &category) const {
    std::vector<Item> items;
    std::vector<int> ids = database.getItemByCategory(category);
    for (auto id : ids) {
        items.push_back(*database.getItemById(id));
    }
    return items;
}

std::vector<Item> ItemService::queryItemsByPrice(const double &min_price,const double &max_price) const {
    std::vector<Item> items;
    // TODO: HOW TO DO WITH DISCOUNT
    std::vector<int> ids = database.getItemByPrice(min_price, max_price);
    for (auto id : ids) {
        items.push_back(*database.getItemById(id));
    }
    return items;
}

Cart ItemService::initCart(const Account &account) const {

    Cart cart(account);
    const auto cart_items = database.getCartItems(account.getId());
    for (const auto &item : cart_items) {
        cart.addCartItem(item);
    }
    return cart;
}

bool ItemService::deleteCart(const int account_id) const {
    return  database.deleteCartItems(account_id);
}

bool ItemService::updateCart(const Cart &cart) const {
    const int account_id = cart.get_account_id();
    // check account
    if (account_id <= 0) {
        return false;
    }

    // remove previous data
    if (!this->deleteCart(account_id)) {
        throw std::runtime_error("Failed to delete cart");
    }

    std::vector<Cart::SomeItems> cart_items;
    auto [itemId_vector, items_map] = cart.get_cart_list();
    for (const int id : itemId_vector) {
        Cart::SomeItems item;
        item.itemId = items_map[id].itemId;
        item.quantity = items_map[id].quantity;
        cart_items.push_back(item);
    }
    return  database.setCartItems(cart_items,account_id);
}

int ItemService::getItemStock(const int item_id) const {
    const std::unique_ptr<Item> item = database.getItemById(item_id);
    return item->get_stock();
}

bool ItemService::updateItemStock(const int item_id, const int stock) const {
    const std::unique_ptr<Item> item = database.getItemById(item_id);
    item->set_stock(stock);
    return database.updateItem(*item);
}

bool ItemService::checkItemStock(const std::vector<Cart::SomeItems> &items) const {
    for (const auto &item : items) {
        const int stock = getItemStock(item.itemId);
        if (item.quantity > stock) {
            std::cout<< item.itemName << " stock: " << stock << std::endl;
            return false;
        }
    }
    return true;
}

bool ItemService::generateOrder(const Account &account, std::vector<Cart::SomeItems> &items,const std::string &address) const {
    Order order(account);
    order.set_address(address);
    for (const auto &item : items) {
        order.push_item(item);
        if (!updateItemStock(item.itemId, getItemStock(item.itemId)-item.quantity)) {
            return false;
        }
    }
    order.set_order_total_price();
    if (!database.addOrder(order)) {
        throw std::runtime_error("Failed to add order");
    }
    autoStatuSwitch(order.get_order_id(),std::chrono::seconds(30));
    return true;
}

bool ItemService::setOrderStatus(Order &order, const int status) const {
    order.set_order_state(status);
    return database.updateOrder(order);
}

bool ItemService::setOrderAddress(Order &order, const std::string &address) const {
    order.set_address(address);
    return database.updateOrder(order);
}

std::vector<Order> ItemService::queryOrderByAccount(const Account &account) const {
    std::vector<Order> orders;
    std::vector<int> order_ids = database.getOrdersByAccount(account);
    for (auto id : order_ids) {
        orders.push_back(*database.getOrderById(id));
    }
    return orders;
}

std::vector<Order> ItemService::queryAllOrders() const {
    std::vector<Order> orders;
    std::vector<int> order_ids = database.queryAllId("user_orders");
    for (auto id : order_ids) {
        orders.push_back(*database.getOrderById(id));
    }
    return orders;
}

void ItemService::autoStatuSwitch(int order_id, std::chrono::seconds delay) const {
    std::jthread([delay, order_id, this]() {
        std::this_thread::sleep_for(delay);
        std::unique_ptr<Order> order = database.getOrderById(order_id);
        if (order->get_order_state() == Order::OrderState::PREPARING) {
            order->set_order_state(2);
            if (!database.updateOrder(*order)) {
                throw std::runtime_error("Failed to update order");
            }
        }
    }).detach();
}

void ItemService::getAllDiscountInfos() const {
    std::vector<int> discount_ids = database.queryAllId("item_discount");

    if (discount_ids.empty()) {
        std::cout << "No discount info" << std::endl;
        return;
    }

    int index = 1;
    for (const auto& discount_id : discount_ids) {
        std::ostringstream oss;
        std::string start_time;
        std::string end_time;
        const std::unique_ptr<Item> item = database.getItemById(discount_id);
        Item::Discount discount = database.getDiscount(discount_id,start_time,end_time);

        oss << "[" << index++ << "]\t";
        oss << item->get_name() << " ";
        if (discount.percent_off!=0)
            oss << discount.percent_off<<" percent off ";
        if (discount.reach!=0)
            oss <<" Reach "<< discount.reach <<"-"<< discount.cut;
        oss<<"\n\t";
        oss << "[Time Limit] "<< start_time << " - " << end_time << std::endl;

        std::cout << oss.str();
    }
}

void ItemService::setDiscount() const {
    const std::unique_ptr<Item> item = database.getItemById(FormMenu::getIntInput("Enter item id: "));
    if ( item == nullptr ){
        FormMenu::noticeTheEnter("Item not found");
        return;
    }

    Item::Discount discount = item->get_discount();
    item->display();

    FormMenu set_discount_menu("Setting discount:");
    set_discount_menu.addItem("set percent-off",
        [&discount]() {
            discount.percent_off = FormMenu::getIntInput("Set percent-off (1-100): ");
        });
    set_discount_menu.addItem("Set reach and cut",
        [&discount]() {
            discount.reach = FormMenu::getIntInput("Set reach:");
            discount.cut = FormMenu::getIntInput("Set cut:");
        });
    set_discount_menu.addItem("Set both",
        [&discount]() {
            discount.percent_off = FormMenu::getIntInput("Set percent-off (1-100): ");
            discount.reach = FormMenu::getIntInput("Set reach:");
            discount.cut = FormMenu::getIntInput("Set cut:");
        });
    set_discount_menu.run();

    int64_t start_time = FormMenu::getTimeInput("Start Time");
    int64_t end_time = FormMenu::getTimeInput("End Time");

    bool isYes = false;
    FormMenu ack_menu("Are you sure to set this discount?");
    ack_menu.addItem("Yes",
        [&isYes]() {
            isYes = true;
        });
    ack_menu.addItem("No",[](){});
    ack_menu.run();

    if (isYes) {
        if (database.setDiscount(item->get_id(),discount,start_time,end_time)) {
            FormMenu::noticeTheEnter("Discount setting succeeded");
        }
        else {
            FormMenu::noticeTheEnter("Discount setting failed");
        }
    }
}

void ItemService::deleteDiscount() const {
    if (database.deleteDiscount(FormMenu::getIntInput("Enter item id: "))) {
        FormMenu::noticeTheEnter("Item deleted succeeded");
    }
    else {
        FormMenu::noticeTheEnter("Item deleted failed");
    }
}

void ItemService::showAllItems(const bool isDetailed) const {
    for (const auto& item : queryAllItems()) {
        item.display(isDetailed);
    }
}


