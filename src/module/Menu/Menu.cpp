//
// Created by dawn on 2025/4/2.
//

#include <utility>

#include "Menu.h"

#include <iostream>
#include <limits>
#include <cstdlib>
Menu::Menu(const std::string &title) :title(title){}

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
    std::cout << "======= " << title << " =======\n";
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << "[" << i + 1 << "] " << items[i].description << std::endl;
    }
    std::cout << "[0] Exit" << std::endl;
    std::cout << "OPTION: ";
}

void Menu::run() const {
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

            // wait users to react
            std::cout << "Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            std::cout << "Invalid choice, enter to try again: " ;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void Menu::setTitle(const std::string &title) {
    this->title = title;
}

