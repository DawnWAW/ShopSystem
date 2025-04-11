//
// Created by dawn on 2025/4/11.
//

#ifndef SHOPMENU_H
#define SHOPMENU_H
#include <ItemService.h>
#include <vector>
#include "Item.h"
#include <cmath>


class ShopMenu final : public Menu{
public:
    struct PageView {
        int item_number = 0;
        int number_per_page = 2;
        int current_page_number = 0;
        int total_pages = 0;
    };

    explicit ShopMenu(ItemService &item_service, int view_state = 0);
    [[nodiscard]] std::string titleToken(const std::string &token= "") const;
    void display() const override;
    void run() const override;
    void showPage() const;
    void nextPage();
    void prevPage();
private:
    ItemService &item_service;
    std::vector<Item> shop_items;
    PageView page_view;
    int view_state = 0; // 0:Browsing 1:all items 2:search by name (and by desc) 3:search by category 4:search by price
};



#endif //SHOPMENU_H
