//
// Created by dawn on 2025/4/2.
//

#ifndef CART_H
#define CART_H
#include <Account.h>
#include "ItemService.h"
#include "Item.h"
#include "FormMenu.h"
#include <vector>
#include <unordered_map>
#include "Menu.h"


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
    explicit Cart(const Account &account);
    void showCart() const;
    bool checkItem(const int item_id) const;
    bool isInList(const int index) const;
    void addCartItem(const Item &item, int quantity);
    void removeCartItem(const int index);
    void removeAllCartItem();
    void updateCartItem(const int index,const int quantity);
    int getCartItemQuantity(const int index) const;
    void showCartMenu() const;
    static int inputItemNumber(const std::string &prompt);
    int inputItemIndex() const;
private:
    Account &account;
    CartList cart_list;
};



#endif //CART_H
