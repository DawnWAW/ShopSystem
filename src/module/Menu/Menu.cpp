//
// Created by dawn on 2025/4/2.
//

#include <utility>

#include "../../../include/Menu.h"

#include <iostream>
#include <limits>
#include <cstdlib>
Menu::Menu(std::string title) :title(std::move(title)){}

void Menu::addItem(const std::string &description, std::function<void()> action) {
    this->items.push_back({description, std::move(action)});
}

void Menu::clearItem() {
    this->items.clear();
}

void Menu::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Menu::display() const {
    clearScreen();
    std::cout << "=== " << title << " ===\n";
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << "[" << i + 1 << "] " << items[i].description << std::endl;
    }
    std::cout << "[0] Back" << std::endl;
    std::cout << "OPTION: ";
}

void Menu::run() const {
    while (true) {
        display();

        int choice;
        std::cin >> choice;

        // clear buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 0) {
            break;
        }

        if (choice > 0 && static_cast<size_t>(choice) <= items.size()) {
            clearScreen();
            items[choice-1].action();

            // wait users to react
            std::cout << std::endl << "Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            std::cout << "Invalid choice, try again" <<std::endl;
        }
    }
}

