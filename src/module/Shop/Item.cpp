//
// Created by dawn on 2025/4/2.
//

#include "Item.h"

#include <iomanip>


// read from database
[[nodiscard]] Item::Item(const int id, const std::string &name, const double price, const int stock, const int state ,const Category category, const std::string &description, const int64_t &created_time, const int64_t &updated_time)
    : id(id),
      name(name),
      category(category),
      description(description),
      price(price),
      stock(stock),
      state(state)
    {
    const time_t created_time_in_seconds = created_time;
    const time_t updated_time_in_seconds = updated_time;
    struct tm *local_created_time = localtime(&created_time_in_seconds);
    struct tm *local_updated_time = localtime(&updated_time_in_seconds);
    char buffer[80];
    strftime(buffer,sizeof(buffer),"%Y-%m-%d %H:%M:%S",local_created_time);
    this->created_time = buffer;
    strftime(buffer,sizeof(buffer),"%Y-%m-%d %H:%M:%S",local_updated_time);
    this->updated_time = buffer;
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
    oss << name << std::endl
        << "category: " << category_to_string(category) << std::endl
        << "price: " << std::fixed << std::setprecision(2) << price << std::endl
        << "stock: " << stock << std::endl
        << "description: " << (description.empty()?"NULL":description) << std::endl
        << "updated Time: " << updated_time << std::endl;
    return oss.str();
}

std::string Item::category_to_string(Category category) {
    switch(category) {
        case Category::GAMES:   return "GAME";
        case Category::DAILY:   return "DAILY";
        case Category::FOOD:    return "FOOD";
        case Category::BOOKS:   return "BOOKS";
        default:                return "OTHER";
    }
}

Item::Category Item::string_to_category(const std::string &category) {
    if (category == "GAMES")    return Category::GAMES;
    if (category == "DAILY")    return Category::DAILY;
    if (category == "FOOD")     return Category::FOOD;
    if (category == "BOOKS")    return Category::BOOKS;
    return Category::OTHER;
}

Item::Category Item::input_category(const std::string &prompt) {
    Item::Category category;
    FormMenu category_menu(prompt);
    for (size_t i = 0; i < 5; i++) {
        category_menu.addItem(Item::category_to_string(static_cast<Item::Category>(i)),
            [&category, i]() {
                category = static_cast<Item::Category>(i);
            });
    }
    category_menu.run();
    return category;
}

double Item::input_price(const std::string &prompt) {
    double price = 0.0;
    do {
        if (price < 0) {
            std::cout << "Price cannot be negative." << std::endl;
        }
        price = FormMenu::getDoubleInput(prompt);
    }while (price < 0);
    return price;

}

int Item::input_stock(const std::string &prompt) {
    int stock = 0;
    do {
        if (stock < 0) {
            std::cout << "Stock cannot be negative." << std::endl;
        }
        stock = FormMenu::getIntInput(prompt);
    }while (stock < 0);
    return stock;
}

int Item::input_state(const std::string &prompt) {
    int state = 0;
    FormMenu saleMenu(prompt);
    saleMenu.addItem("On sale",
        [&state](){ state = 1; });
    saleMenu.addItem("Not on sale",
        [&state](){ state = 0; });
    saleMenu.run();
    return state;
}


