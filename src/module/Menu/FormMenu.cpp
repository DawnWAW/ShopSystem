//
// Created by dawn on 2025/4/10.
//

#include "FormMenu.h"

FormMenu::FormMenu(const std::string &prompt):Menu(prompt) {
}

void FormMenu::display() const {
    std::cout << "=== " << title << " ===\n";
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << "[" << i + 1 << "] " << items[i].description << std::endl;
    }
    std::cout << "OPTION: ";
}

void FormMenu::run() const {
    int choice;
    display();
    while (true) {
        std::cin >> choice;
        // clear buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice > 0 && static_cast<size_t>(choice) <= items.size()) {
            items[choice-1].action();
            return;
        }
        else {
            std::cout << "Invalid, try again: " ;
        }
    }
}

std::string FormMenu::getStrInput(const std::string &prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

double FormMenu::getDoubleInput(const std::string &prompt) {
    double input;
    std::cout << prompt;
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

int FormMenu::getIntInput(const std::string &prompt) {
    int input;
    std::cout << prompt;
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}
