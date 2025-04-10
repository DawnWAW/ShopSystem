//
// Created by dawn on 2025/4/10.
//

#include "ItemService.h"
ItemService::ItemService(Database &database) : database(database) {}

void ItemService::addItem() const {
    std::cout << "Adding item: " << std::endl;
    // 1. item name
    std::string itemName = FormMenu::getStrInput("Enter item name: ");

    // 2. item category
    Item::Category category;
    FormMenu category_menu("Choose item category: ");
    for (size_t i = 0; i < 5; i++) {
        category_menu.addItem(Item::category_to_string(static_cast<Item::Category>(i)),
            [&category, i]() {
                category = static_cast<Item::Category>(i);
            });
    }
    category_menu.run();

    // 3. description
    std::string description = FormMenu::getStrInput("Enter item description (can be null): ");

    // 4. price
    double price = FormMenu::getDoubleInput("Enter item price: ");

    // 5. stock
    int stock = FormMenu::getIntInput("Enter item stock: ");

    // 6. sale state
    int state;
    FormMenu saleMenu("Choose item sale state: ");
    saleMenu.addItem("On sale",
        [&state](){ state = 1; });
    saleMenu.addItem("Not on sale",
        [&state](){ state = 0; });

    // 7. show item info
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
        [this, &itemName, &category, &category_menu, &description, &price, &stock, &state]() {
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
