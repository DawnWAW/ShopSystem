//
// Created by dawn on 2025/4/11.
//

#include "ShopMenu.h"


ShopMenu::ShopMenu(ItemService &item_service, const int view_state):  item_service(item_service),view_state(view_state) {
    shop_items = item_service.queryAllItems();
    page_view.item_number = static_cast<int>(shop_items.size());
    page_view.total_pages =  std::ceil(static_cast<double>(page_view.item_number)/page_view.number_per_page);
    setTitle(titleToken());
}

std::string ShopMenu::titleToken(const std::string &token ) const {
    switch (view_state) {
        case 0: return "Browsing";
        case 1: return "Shopping: all items";
        case 2: return "Searching by name: " + token;
        case 3: return "Searching by category: " + token;
        case 4: return "Searching by price: " + token;
        default: return "Shopping";
    }
}

void ShopMenu::display() const {
    clearScreen();
    std::cout << "======= " << title << " =======\n";
    showPage();
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << "[" << i + 1 << "] " << items[i].description << std::endl;
    }
    std::cout << "[0] Exit" << std::endl;
    std::cout << "OPTION: ";
}

void ShopMenu::run() const {
    std::string input;
    while (true) {
        display();
        std::getline(std::cin, input);
        if (input.length() > 1 || input.empty()) {
            std::cout << "Invalid choice, enter to try again" ;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        int choice = input.at(0) - '0';
        if (choice == 0) {
            break;
        }
        if (choice > 0 && static_cast<size_t>(choice) <= items.size()) {
            clearScreen();
            items[choice-1].action();
        }
        else {
            std::cout << "Invalid choice, enter to try again: " ;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void ShopMenu::showPage() const {
    const int from = page_view.current_page_number * page_view.number_per_page;
    const int to = (from + page_view.number_per_page > page_view.item_number) ? page_view.item_number : from + page_view.number_per_page;
    for (int i = from; i < to; ++i) {
        std::cout << "Item index: [" << i+1 << "]" << std::endl;
        shop_items[i].display();
    }
}

void ShopMenu::nextPage() {
    if (page_view.current_page_number == page_view.total_pages - 1) {
        page_view.current_page_number = 0;
    }
    else
        this->page_view.current_page_number++;
}

void ShopMenu::prevPage() {
    if (page_view.current_page_number == 0) {
        page_view.current_page_number = page_view.total_pages-1;
    }
    else
        this->page_view.current_page_number--;
}
