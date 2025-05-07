//
// Created by dawn on 2025/4/10.
//

#ifndef FORMMENU_H
#define FORMMENU_H
#include <Menu.h>
#include <iostream>
#include <limits>


class FormMenu final : public Menu {
public:
    explicit FormMenu(const std::string &prompt);
    void display() const;
    void run() const;
    static std::string getStrInput(const std::string &prompt);
    static double getDoubleInput(const std::string &prompt);
    static int getIntInput(const std::string &prompt);
    static void noticeTheEnter(const std::string &prompt);
};



#endif //FORMMENU_H
