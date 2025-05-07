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

bool Cart::isCartEmpty() const {
    return this->cart_list.itemId_vector.empty();
}

bool Cart::is_cart_modified() const {
    return isCartModified;
}

void Cart::showCart() const {
    if (this->isCartEmpty()) {
        FormMenu::noticeTheEnter("My cart is Empty");
        return;
    }

    int total_number = 0;
    double total_price = 0;
    for (int i=0;i<cart_list.itemId_vector.size();++i) {
        CartItem item = cart_list.items_map.at(cart_list.itemId_vector.at(i));
        std::cout << "[" << i+1 << "]"
        << item.itemName
        << " [Price] " << item.itemPrice <<"$ "
        << " [Quantity] " << item.quantity
        << std::endl;

        total_number += item.quantity;
        total_price += item.quantity*item.itemPrice;
    }
    std::cout << "[Total number] " << total_number << std::endl;
    std::cout << "[Total price] " << total_price << std::endl;
}

bool Cart::checkItem(const int item_id) const {
        return this->cart_list.items_map.contains(item_id);
}

bool Cart::isInList(const int index) const {
    return this->cart_list.itemId_vector.size() > index && index>=0;
}


void Cart::addCartItem(const Item &item, const int quantity) {
    CartItem new_item;
    new_item.itemId = item.get_id();
    new_item.itemName = item.get_name();
    new_item.itemPrice = item.get_price();
    new_item.quantity = quantity;

    this->cart_list.itemId_vector.push_back(new_item.itemId);
    this->cart_list.items_map[new_item.itemId] = new_item;
    this->isCartModified = true;
}

void Cart::addCartItem(const CartItem &cart_item) {
    this->cart_list.itemId_vector.push_back(cart_item.itemId);
    this->cart_list.items_map[cart_item.itemId] = cart_item;
}

void Cart::removeCartItemById(const int id) {
    std::erase(this->cart_list.itemId_vector,id);
    this->cart_list.items_map.erase(id);
    this->isCartModified = true;
}

void Cart::removeCartItem(const int index) {
    removeCartItemById(this->cart_list.itemId_vector[index]);
    this->isCartModified = true;
}

void Cart::removeAllCartItem() {
    for (const int &id :  this->cart_list.itemId_vector) {
        this->removeCartItemById(id);
    }
    this->isCartModified = true;
}

void Cart::updateCartItem(const int index, const int quantity) {
    this->cart_list.items_map[this->cart_list.itemId_vector[index]].quantity = quantity;
    this->isCartModified = true;
}

int Cart::getCartItemQuantity(const int index) const {
    return this->cart_list.items_map.at(this->cart_list.itemId_vector[index]).quantity;
}

int Cart::inputItemIndex() const {
    this->showCart();
    while (true) {
        int index = FormMenu::getIntInput("Enter index: ") - 1;
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


