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
    Item::Category category = Item::input_category("Choose item category: ");

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
    << "Category: " << Item::category_to_string(category) << std::endl
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
    if ( item != nullptr ) {
        std::cout << "Item info:" << std::endl;
        item->display();
    }
    else {
        std::cout << "Item not found." << std::endl;
        return;
    }

    Menu updateMenu("Update item options: ");
    updateMenu.addItem("Choose other item",
        [&item,this]() {
            item = database.getItemById(FormMenu::getIntInput("Enter item id: "));

            if ( item != nullptr ) {
              std::cout << "Item info:" << std::endl;
              item->display();
            }
            else {
              std::cout << "Item not found." << std::endl;
            }
        });
    updateMenu.addItem("Update name",
        [&item]() {
            item->set_name(Item::input_name("Enter new item name: "));
            item->display();
        });
    updateMenu.addItem("Update price",
                       [&item]() {
                           item->set_price(Item::input_price("Enter new item price: "));
                           item->display();
                       });
    updateMenu.addItem("Update stock",
                       [&item]() {
                           item->set_stock(Item::input_stock("Enter new item stock: "));
                           item->display();
                       });
    updateMenu.addItem("Update state",
                       [&item]() {
                           item->set_state(Item::input_state("Choose new item state: "));
                           item->display();
                       });
    updateMenu.addItem("Update category",
                       [&item]() {
                            item->set_category(Item::input_category("Choose new item category: "));
                            item->display();
                       });
    updateMenu.addItem("Update description",
                       [&item]() {
                           item->set_description(FormMenu::getStrInput("Enter new item description: "));
                           item->display();
                       });
    updateMenu.addItem("Submit",
        [&item, this]() {
            if (database.updateItem(*item)) {
                item->display();
                std::cout << "Item update succeeded" << std::endl;
            }
            else {
                std::cout << "Item update failed" << std::endl;
            }
        });
    updateMenu.run();
}

std::vector<Item> ItemService::queryAllItems() const {
    std::vector<Item> items;
    std::vector<int> ids = database.queryAllId("items");
    for (auto id : ids) {
        items.push_back(*database.getItemById(id));
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

[[nodiscard]] std::vector<Item> ItemService::queryItemsByCategory(Item::Category category) const {
    std::vector<Item> items;
    std::vector<int> ids = database.getItemByCategory(category);
    for (auto id : ids) {
        items.push_back(*database.getItemById(id));
    }
    return items;
}

std::vector<Item> ItemService::queryItemsByPrice(const double &min_price,const double &max_price) const {
    std::vector<Item> items;
    std::vector<int> ids = database.getItemByPrice(min_price, max_price);
    for (auto id : ids) {
        items.push_back(*database.getItemById(id));
    }
    return items;
}

void ItemService::showAllItems(const bool isDetailed) const {
    for (const auto& item : queryAllItems()) {
        item.display(isDetailed);
    }
}


