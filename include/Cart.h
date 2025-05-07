//
// Created by dawn on 2025/4/2.
//

#ifndef CART_H
#define CART_H
#include <Account.h>
#include "Item.h"
#include "FormMenu.h"
#include <vector>
#include <unordered_map>
#include "Menu.h"
#include <set>


class Cart {
public:
    struct CartItem {
        int itemId;
        std::string itemName;
        double itemPrice;
        int quantity;
    };
    struct CartList {
        std::vector<int> itemId_vector;
        std::unordered_map<int, CartItem> items_map;
    };

    explicit Cart(Account account);
    [[nodiscard]] CartList get_cart_list() const;
    int get_account_id() const;
    bool isCartEmpty() const;
    [[nodiscard]] bool is_cart_modified() const;
    void showCart() const;
    bool checkItem(const int item_id) const;
    bool isInList(const int index) const;
    void addCartItem(const Item &item, int quantity);
    void addCartItem(const CartItem &cart_item);
    void removeCartItemById(int id);
    void removeCartItem(const int index);
    void removeAllCartItem();
    void updateCartItem(const int index,const int quantity);
    int getCartItemQuantity(const int index) const;
    static int inputItemNumber(const std::string &prompt);
    int inputItemIndex() const;

private:
    Account account;
    CartList cart_list;
    bool isCartModified = false;
};



#endif //CART_H
