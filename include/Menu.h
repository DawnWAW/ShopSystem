//
// Created by dawn on 2025/4/2.
//

#ifndef MENU_H
#define MENU_H
#include <functional>
#include <string>
#include <vector>


class Menu {
public:
    struct MenuItem {
        std::string description;
        std::function<void()> action;
    };

    explicit Menu(std::string title);

    void addItem(const std::string &description, std::function<void()> action);
    void clearItem();
    void display() const;
    void run() const;
    void setTitle(const std::string &title);

private:
    std::string title;
    std::vector<MenuItem> items;
    static void clearScreen() ;


};



#endif //MENU_H
