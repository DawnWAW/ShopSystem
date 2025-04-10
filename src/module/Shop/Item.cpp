//
// Created by dawn on 2025/4/2.
//

#include "Item.h"

#include <iomanip>


// read from database
[[nodiscard]] Item::Item(const int id, const std::string &name, const double price, const int stock, const int state ,const Category category, const std::string &description, const std::string &created_time, const std::string &updated_time)
    : id(id),
      name(name),
      category(category),
      description(description),
      price(price),
      stock(stock),
      state(state),
      created_time(created_time),
      updated_time(updated_time) {
}

// create by admin
Item::Item(const std::string &name, double price, int stock, int state, Category category, const std::string &description)
    : name(name),
      category(category),
      description(description),
      price(price),
      stock(stock),
      state(state)
    {
    id = 0;
}


[[nodiscard]] int Item::get_id() const {
    return id;
}

void Item::set_id(const int id) {
    this->id = id;
}

[[nodiscard]] std::string Item::get_name() const {
    return name;
}

void Item::set_name(const std::string &name) {
    this->name = name;
}

[[nodiscard]] Item::Category Item::get_category() const {
    return category;
}

void Item::set_category(const Item::Category category) {
    this->category = category;
}

[[nodiscard]] std::string Item::get_description() const {
    return description;
}

void Item::set_description(const std::string &description) {
    this->description = description;
}

[[nodiscard]] double Item::get_price() const {
    return price;
}

void Item::set_price(const double price) {
    this->price = price;
}

[[nodiscard]] int Item::get_stock() const {
    return stock;
}

void Item::set_stock(const int stock) {
    this->stock = stock;
}

[[nodiscard]] int Item::get_state() const {
    return state;
}

void Item::set_state(const int state) {
    this->state = state;
}

[[nodiscard]] std::string Item::get_created_time() const {
    return created_time;
}

[[nodiscard]] std::string Item::get_updated_time() const {
    return updated_time;
}

void Item::display() const {
    std::cout << to_string();
}

std::string Item::to_string() const {
    std::ostringstream oss;
    oss << "["<< id << "]\t" << name << "(" << category_to_string(category) << ")" <<std::endl
        << "<< " << std::fixed << std::setprecision(2) << price << "$ >>"
        << "{{ " << stock << " left }} " << std::endl
        << "Description: " << (description.empty()?"NULL":description) << std::endl
        << "Update Time: " << updated_time << std::endl;
    return oss.str();
}

std::string Item::category_to_string(Category category) {
    switch(category) {
        case Category::GAMES: return "GAME";
        case Category::DAILY:    return "DAILY";
        case Category::FOOD:        return "FOOD";
        case Category::BOOKS:       return "BOOKS";
        default:                    return "OTHER";
    }
}
