//
// Created by dawn on 2025/4/2.
//

#include "Cart.h"

#include <utility>


Cart::Cart(Account account): account(std::move(account)) {
}

[[nodiscard]] Cart::CartList Cart::get_cart_list() const {
    return cart_list;
}

int Cart::get_account_id() const {
    return this->account.getId();
}

void Cart::showCart() const {
    int total_number = 0;
    double total_price = 0;
    for (const int id : cart_list.itemId_vector) {
        CartItem item = cart_list.items_map.at(id);
        std::cout << "[" << id << "] "
        << item.itemName << " $"
        << item.itemPrice << " *"
        << item.quantity << std::endl;

        total_number += item.quantity;
        total_price += item.quantity*item.itemPrice;
    }
    std::cout << "Total number: " << total_number << std::endl;
    std::cout << "Total price: " << total_price << std::endl;
}

bool Cart::checkItem(const int item_id) const {
        return this->cart_list.items_map.contains(item_id);
}

bool Cart::isInList(const int index) const {
    return this->cart_list.itemId_vector.size() > index;
}


void Cart::addCartItem(const Item &item, int quantity) {
    CartItem new_item;
    new_item.itemId = item.get_id();
    new_item.itemName = item.get_name();
    new_item.itemPrice = item.get_price();
    new_item.quantity = quantity;

    this->cart_list.itemId_vector.push_back(new_item.itemId);
    this->cart_list.items_map[new_item.itemId] = new_item;
}

void Cart::addCartItem(const CartItem &cart_item) {
    this->cart_list.itemId_vector.push_back(cart_item.itemId);
    this->cart_list.items_map[cart_item.itemId] = cart_item;
}

void Cart::removeCartItem(const int index) {
    const int id = cart_list.itemId_vector.at(index);
    this->cart_list.itemId_vector.erase(this->cart_list.itemId_vector.begin() + index);
    this->cart_list.items_map.erase(id);
}

void Cart::removeAllCartItem() {
    for (const int &index :  this->cart_list.itemId_vector) {
        this->removeCartItem(index);
    }
}

void Cart::updateCartItem(const int index, const int quantity) {
    this->cart_list.items_map[this->cart_list.itemId_vector[index]].quantity = quantity;
}

int Cart::getCartItemQuantity(const int index) const {
    return this->cart_list.items_map.at(this->cart_list.itemId_vector[index]).quantity;
}

void Cart::showCartMenu() const {
    Menu cart_menu("My cart");
    cart_menu.addItem("Cart item: ",
        [this]() {
            if (int cart_id = FormMenu::getIntInput("Enter cart id: "); this->cart_list.items_map.contains(cart_id)) {
                std::cout << "Item is already in your cart, do you want to ";
            }
        });

}

int Cart::inputItemIndex() const {
    int index;
    this->showCart();
    while (true) {
        index = FormMenu::getIntInput("Enter index: ");
        if (this->isInList(index)) {
            return index;
        }
        std::cout << "Invalid index, try again" << std::endl;
    }
}

int Cart::inputItemNumber(const std::string &prompt){
    int item_number = -1;
    while (item_number < 0) {
        item_number=FormMenu::getIntInput(prompt);
        if (item_number < 0) {
            std::cout << "Item number should more than 0" << std::endl;
        }
    }
    return item_number;
}


