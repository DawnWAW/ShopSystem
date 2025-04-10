//
// Created by dawn on 2025/4/2.
//

#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <sstream>
#include <iostream>



class Item {

public:
    enum class Category {
        GAMES,
        FOOD,
        BOOKS,
        DAILY,
        OTHER
    };


    // read from database
    [[nodiscard]] Item(const int id, const std::string &name, const double price, const int stock,const int state, const Category category, const std::string &description, const std::string &created_time, const std::string &updated_time);


    // create by admin
    Item(const std::string &name, double price, int stock, const int state, Category category = Category::OTHER, const std::string &description = "");

private:
    int id;

    std::string name;

    Category category;

    std::string description;

    double price;

    int stock;

    int state;

private:
    std::string created_time;

    std::string updated_time;

public:

    [[nodiscard]] int get_id() const;

    void set_id(int id);

    [[nodiscard]] std::string get_name() const;

    void set_name(const std::string &name);

    [[nodiscard]] Category get_category() const;

    void set_category(Category category);

    [[nodiscard]] std::string get_description() const;

    void set_description(const std::string &description);

    [[nodiscard]] double get_price() const;

    void set_price(double price);

    [[nodiscard]] int get_stock() const;

    void set_stock(int stock);

    int get_state() const;

    void set_state(int state);

    [[nodiscard]] std::string get_created_time() const;

    bool increaseStock(int amount);

    bool decreaseStock(int amount);

    [[nodiscard]] std::string get_updated_time() const;

    void display() const;

    [[nodiscard]] std::string to_string() const;

    static std::string category_to_string(Category category);
};



#endif //ITEM_H
