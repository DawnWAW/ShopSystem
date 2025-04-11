//
// Created by dawn on 2025/4/10.
//

#ifndef ITEMSERVICE_H
#define ITEMSERVICE_H
#include "Database.h"
#include "Item.h"
#include "FormMenu.h"
#include <iostream>
#include <limits>
#include <vector>


class ItemService {
private:
  Database &database;
public:
  explicit ItemService(Database &database);

  void addItem() const;

  void updateItem() const;

  [[nodiscard]] std::vector<Item> queryAllItems() const;

  void showAllItems() const;
};



#endif //ITEMSERVICE_H
