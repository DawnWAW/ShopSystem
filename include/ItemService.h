//
// Created by dawn on 2025/4/10.
//

#ifndef ITEMSERVICE_H
#define ITEMSERVICE_H


#include "Database.h"
#include "Item.h"
#include "Cart.h"
#include "FormMenu.h"
#include <iostream>
#include <limits>
#include <vector>
#include <thread>
#include <chrono>


class ItemService {
private:
  Database &database;
public:
  explicit ItemService(Database &database);

  // admin interfaces
  void addItem() const;

  void deleteItem() const;

  void updateItem() const;

  [[nodiscard]] std::vector<Item> queryAllItems() const;

  // shopping interface
  void showAllItems(bool isDetailed = false) const;

  [[nodiscard]] std::vector<Item> queryItemsByName(const std::string &name) const;

  [[nodiscard]] std::vector<Item> queryItemsByCategory(const std::string &category) const;

  [[nodiscard]] std::vector<Item> queryItemsByPrice(const double &min_price, const double &max_price) const;

  // cart interface
  [[nodiscard]] Cart initCart(const Account &account) const;

  [[nodiscard]] bool deleteCart(const int account_id) const;

  [[nodiscard]] bool updateCart(const Cart &cart) const;

  // stock control
  [[nodiscard]] int getItemStock(const int item_id) const;

  [[nodiscard]] bool updateItemStock(const int item_id, const int stock) const;

  // order interface
  bool checkItemStock(const std::vector<Cart::SomeItems> &items) const;

  bool generateOrder(const Account &account, std::vector<Cart::SomeItems> &items,const std::string &address) const;

  bool setOrderStatus(Order &order, int status) const;

  bool setOrderAddress(Order &order, const std::string &address) const;

  [[nodiscard]] std::vector<Order> queryOrderByAccount(const Account &account) const;

  [[nodiscard]] std::vector<Order> queryAllOrders() const;

  void autoStatuSwitch(int order_id, std::chrono::seconds delay) const;
};



#endif //ITEMSERVICE_H
