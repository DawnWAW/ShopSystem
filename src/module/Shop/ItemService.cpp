//
// Created by dawn on 2025/4/10.
//

#include "ItemService.h"
ItemService::ItemService(Database &database) : database(database) {}

void ItemService::addItem() const {
    std::cout << "Adding item: " << std::endl;
    std::string itemName;
    std::string description;
    double price;
    int stock;
    int choice;

    std::cout << "Enter item name: ";
    std::getline(std::cin, itemName);

    for (size_t i = 0; i < 5; i++) {
        std::cout << "["<< i+1 <<"]" << Item::category_to_string(static_cast<Item::Category>(i)) << std::endl;
    }
    std::cout << "Choose item category: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (choice <= 0 || choice > 5) { choice = 5; }
    auto itemCategory = static_cast<Item::Category>(choice - 1);

    std::cout << "Enter item description (can be null): ";
    std::getline(std::cin, description);

    std::cout << "Enter item price: ";
    std::cin >> price;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter item stock: ";
    std::cin >> stock;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "[1] On sale\t";
    std::cout << "[2] Off sale\n";
    std::cout << "Enter item state: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    int state = choice == 1 ? 1 : 0;


    std::cout << "Item Info: " <<std::endl
    << "ItemName: " << itemName << std::endl
    << "Category: " << Item::category_to_string(itemCategory) << std::endl
    << "Description: " << description << std::endl
    << "Price: " << price << std::endl
    << "Stock: " << stock << std::endl;
    std::cout << "State: ";
    if (state) {
        std::cout << "On sale" << std::endl;
    }else {
        std::cout << "NOt on sale" << std::endl;
    }
    std::cout <<"Are you sure to add this item?(y/n)";
    std::string ack;
    getline(std::cin, ack);
    if (ack == "y" || ack == "Y") {
        Item item(itemName,price,stock,state,itemCategory,description);
        if(database.addItem(item)) {
            std::cout << "Item adding succeeded" << std::endl;
        }
        else {
            std::cout << "Item adding failed" << std::endl;
        }
    }
    else {
        std::cout << "Item adding canceled" << std::endl;
    }
}
