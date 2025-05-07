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

    explicit Menu(const std::string& title = "");
    void addItem(const std::string &description, std::function<void()> action);
    void clearItem();
    virtual void display() const;
    virtual void display(const std::function<void()>& func) const;
    virtual void run() const;
    virtual void run(const std::function<void()>& func) const;
    void setTitle(const std::string &title);
    static void clearScreen() ;

protected:
    std::string title;
    std::vector<MenuItem> items;
};



#endif //MENU_H
