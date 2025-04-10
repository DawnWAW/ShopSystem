//
// Created by dawn on 2025/4/10.
//

#ifndef ITEMSERVICE_H
#define ITEMSERVICE_H
#include "Database.h"
#include "Item.h"
#include <iostream>
#include <limits>


class ItemService {
private:
  Database &database;
public:
  ItemService(Database &database);

  void addItem() const;
};



#endif //ITEMSERVICE_H
