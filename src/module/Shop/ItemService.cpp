//
// Created by dawn on 2025/4/10.
//

#include "ItemService.h"
ItemService::ItemService(Database &database) : database(database) {}

/// 管理员添加物品
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

/// 管理员删除物品
void ItemService::deleteItem() const {
    // 根据ID获取要删除的物品
    std::unique_ptr<Item> item = database.getItemById(FormMenu::getIntInput("Enter item id: "));
    if ( item == nullptr ) {
        std::cout << "Item not found." << std::endl;
        return;
    }

    // 展示物品并提供确认删除的选项
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

/// 管理员更改物品
void ItemService::updateItem() const {
    // 获取对应物品
    std::unique_ptr<Item> item = database.getItemById(FormMenu::getIntInput("Enter item id: "));
    if ( item == nullptr ){
        FormMenu::noticeTheEnter("Item not found");
        return;
    }
    // 创建菜单提供对应的setter方法
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
    // 将完成修改的item提交到数据库中完成更改
    updateMenu.addItem("Submit",
                        [&item, this]() {
                            // 调用数据库接口
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

/// 查询所有物品
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

/// 根据名字查询物品
[[nodiscard]] std::vector<Item> ItemService::queryItemsByName(const std::string &name) const {
    std::vector<Item> items;
    std::vector<int> ids = database.getItemByName(name);
    for (auto id : ids) {
        items.push_back(*database.getItemById(id));
    }
    return items;
}

/// 根据类别查询物品
[[nodiscard]] std::vector<Item> ItemService::queryItemsByCategory(const std::string &category) const {
    std::vector<Item> items;
    std::vector<int> ids = database.getItemByCategory(category);
    for (auto id : ids) {
        items.push_back(*database.getItemById(id));
    }
    return items;
}

/// 根据价格查询物品
std::vector<Item> ItemService::queryItemsByPrice(const double &min_price,const double &max_price) const {
    std::vector<Item> items;
    // TODO: HOW TO DO WITH DISCOUNT
    std::vector<int> ids = database.getItemByPrice(min_price, max_price);
    for (auto id : ids) {
        items.push_back(*database.getItemById(id));
    }
    return items;
}

/// 加载某个用户的购物车
/// @param account 购物车所属的用户
/// @return 购物车
Cart ItemService::initCart(const Account &account) const {

    // 构造一个空购物车
    Cart cart(account);
    // 从数据库中查询该用户的购物车信息
    const auto cart_items = database.getCartItems(account.getId());
    for (const auto &item : cart_items) {
        cart.addCartItem(item);
    }
    return cart;
}

/// 删除数据库中存储的购物车的数据, 用于更新购物车
/// @param account_id 购物车对应用户的ID
/// @return 删除是否成功
bool ItemService::deleteCart(const int account_id) const {
    return  database.deleteCartItems(account_id);
}

/// 更新数据库中购物车的数据
/// @param cart 需要更新到数据库的购物车
/// @return 更新是否成功
bool ItemService::updateCart(const Cart &cart) const {
    // get user id
    const int account_id = cart.get_account_id();
    // check account
    if (account_id <= 0) {
        return false;
    }

    // remove previous data
    if (!this->deleteCart(account_id)) {
        throw std::runtime_error("Failed to delete cart");
    }

    // load items in cart to the vector for inserting into database
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


/// 获取物品的库存
/// @param item_id 物品的ID
/// @return 物品的库存
int ItemService::getItemStock(const int item_id) const {
    const std::unique_ptr<Item> item = database.getItemById(item_id);
    return item->get_stock();
}

/// 更改物品库存
/// @param item_id 物品的ID
/// @param stock 更改的库存数
/// @return 更改是否成功
bool ItemService::updateItemStock(const int item_id, const int stock) const {
    const std::unique_ptr<Item> item = database.getItemById(item_id);
    item->set_stock(stock);
    return database.updateItem(*item);
}

/// 检查vector中购买的物品的数量是否超出库存
/// @param items 订单中的物品
/// @return 是否有物品购买的数量超出库存
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

/// 生成订单
/// @param account 订单购买者
/// @param items 订单购买的物品
/// @param address 订单地址
/// @return 订单是否成功生成
bool ItemService::generateOrder(const Account &account, std::vector<Cart::SomeItems> &items,const std::string &address) const {
    // 初始化订单和地址信息
    Order order(account);
    order.set_address(address);
    // 更新购买物品的库存
    for (const auto &item : items) {
        order.push_item(item);
        if (!updateItemStock(item.itemId, getItemStock(item.itemId)-item.quantity)) {
            return false;
        }
    }
    // 计算物品的总价格
    order.set_order_total_price();
    // 数据库存储订单
    if (!database.addOrder(order)) {
        throw std::runtime_error("Failed to add order");
    }
    // 自动转换订单状态
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

/// 自动转换订单状态
/// @param order_id 订单的ID
/// @param delay 自动转换的延迟时间
void ItemService::autoStatuSwitch(int order_id, std::chrono::seconds delay) const {
    // 创建一个线程
    std::jthread([delay, order_id, this]() {
        // 线程休眠设定好的时间
        std::this_thread::sleep_for(delay);
        // 获取对应订单并查询订单状态是否为待发货
        std::unique_ptr<Order> order = database.getOrderById(order_id);
        if (order->get_order_state() == Order::OrderState::PREPARING) {
            // 更改订单状态
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

    for (const auto& discount_id : discount_ids) {
        std::ostringstream oss;
        std::string start_time;
        std::string end_time;
        const std::unique_ptr<Item> item = database.getItemById(discount_id);
        Item::Discount discount = database.getDiscount(discount_id,start_time,end_time);

        oss << "[" << discount_id << "]\t";
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

    Item::Discount discount;
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


