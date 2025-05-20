//
// Created by dawn on 2025/4/2.
//

#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <ctime>
#include <sstream>
#include <iostream>
#include "FormMenu.h"



class Item {

public:
    enum class Category {
        GAMES,
        FOOD,
        BOOKS,
        DAILY,
        OTHER
    };

    struct Discount {
        int percent_off = 0;
        double reach = 0;
        double cut = 0;
    };


    // read from database
    [[nodiscard]] Item(const int id, const std::string &name, const double price, const int stock,const int state, const std::string category, const std::string &description, const int64_t &created_time, const int64_t &updated_time);


    // create by admin
    Item(const std::string &name, double price, int stock, const int state, std::string category, const std::string &description = "");

private:
    int id;

    std::string name;

    std::string category;

    std::string description;

    double price;

    int stock;

    int state;

    Discount discount;

private:
    std::string created_time;

    std::string updated_time;

public:

    [[nodiscard]] int get_id() const;

    void set_id(int id);

    [[nodiscard]] std::string get_name() const;

    void set_name(const std::string &name);

    [[nodiscard]] std::string get_category() const;

    void set_category(const std::string &category);

    [[nodiscard]] std::string get_description() const;

    void set_description(const std::string &description);

    [[nodiscard]] double get_price() const;

    [[nodiscard]] double get_discount_price() const;

    void set_price(double price);

    [[nodiscard]] Discount get_discount() const;

    void set_discount(const Discount &discount);

    [[nodiscard]] int get_stock() const;

    void set_stock(int stock);

    int get_state() const;

    void set_state(int state);

    [[nodiscard]] std::string get_created_time() const;

    [[nodiscard]] std::string get_updated_time() const;

    void display(bool isDetailed = false) const;

    [[nodiscard]] std::string to_string(bool isDetailed) const;

    static std::string category_to_string(Category category);

    [[nodiscard]] static Category string_to_category(const std::string &category) ;

    [[ nodiscard ]] static std::string input_name(const std::string &prompt);

    [[nodiscard]] static std::string input_category(const std::string &prompt);

    [[nodiscard]] static double input_price(const std::string &prompt);

    [[nodiscard]] static int input_stock(const std::string &prompt);

    [[nodiscard]] static int input_state(const std::string &prompt);
};



#endif //ITEM_H
