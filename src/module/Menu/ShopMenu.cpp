//
// Created by dawn on 2025/4/11.
//

#include "ShopMenu.h"


ShopMenu::ShopMenu(ItemService &item_service, const MainMenu::AppState &app_state):
item_service(item_service),view_state(app_state.isLoggedIn ? 1 : 0),cart(app_state.account) {
    shop_items = item_service.queryAllItems();
    page_view.item_number = static_cast<int>(shop_items.size());
    page_view.total_pages =  std::ceil(static_cast<double>(page_view.item_number)/page_view.number_per_page);
    setTitle(titleToken());
    if (app_state.isLoggedIn) {
        this->cart = item_service.initCart(app_state.account);
    }
}

std::string ShopMenu::titleToken(const std::string &token ) const {
    switch (view_state) {
        case 0: return "Browsing";
        case 1: return "Shopping: all items";
        case 2: return "Searching by name: " + token;
        case 3: return "Searching by category: " + token;
        case 4: return "Searching by price: " + token;
        default: return "Shopping";
    }
}

void ShopMenu::display() const {
    clearScreen();
    std::cout << "======= " << title
    << " [" << this->page_view.current_page_number+1
    << "/" << page_view.total_pages << "]"
    << " =======\n";
    showPage();
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << "[" << i + 1 << "] " << items[i].description << std::endl;
    }
    std::cout << "[0] Exit" << std::endl;
    std::cout << "OPTION: ";
}

void ShopMenu::run() const {
    std::string input;
    while (true) {
        display();
        std::getline(std::cin, input);
        if (input.length() > 1 || input.empty()) {
            std::cout << "Invalid choice, enter to try again" ;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        int choice = input.at(0) - '0';
        if (choice == 0) {
            break;
        }
        if (choice > 0 && static_cast<size_t>(choice) <= items.size()) {
            clearScreen();
            items[choice-1].action();
        }
        else {
            std::cout << "Invalid choice, enter to try again: " ;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void ShopMenu::showPage() const {
    if (shop_items.empty()) {
        std::cout << "No item" << std::endl;
        return;
    }
    const int from = page_view.current_page_number * page_view.number_per_page;
    const int to = (from + page_view.number_per_page > page_view.item_number) ? page_view.item_number : from + page_view.number_per_page;
    for (int i = from; i < to; ++i) {
        std::cout << "Item index: [" << i+1 << "]" << std::endl;
        shop_items[i].display();
    }
}

void ShopMenu::nextPage() {
    if (page_view.current_page_number == page_view.total_pages - 1) {
        page_view.current_page_number = 0;
    }
    else
        this->page_view.current_page_number++;
}

void ShopMenu::prevPage() {
    if (page_view.current_page_number == 0) {
        page_view.current_page_number = page_view.total_pages-1;
    }
    else
        this->page_view.current_page_number--;
}

void ShopMenu::showAllItems(const bool isLoggedIn) {
    this->shop_items = this->item_service.queryAllItems();
    page_view.item_number = static_cast<int>(shop_items.size());
    page_view.current_page_number = 0;
    page_view.total_pages =  std::ceil(static_cast<double>(page_view.item_number)/page_view.number_per_page);
    this->view_state = isLoggedIn?1:0;
    setTitle(titleToken());
}

void ShopMenu::searchByName(){
    const std::string item_name = Item::input_name("Search item by name: ");
    this->shop_items = this->item_service.queryItemsByName(item_name);
    page_view.item_number = static_cast<int>(shop_items.size());
    page_view.current_page_number = 0;
    page_view.total_pages =  std::ceil(static_cast<double>(page_view.item_number)/page_view.number_per_page);
    this->view_state = 2;
    setTitle(titleToken(item_name));
}

void ShopMenu::searchByCategory() {
    const auto category = Item::input_category("Search item by category: ");
    this->shop_items = this->item_service.queryItemsByCategory(category);
    page_view.item_number = static_cast<int>(shop_items.size());
    page_view.current_page_number = 0;
    page_view.total_pages =  std::ceil(static_cast<double>(page_view.item_number)/page_view.number_per_page);
    this->view_state = 3;
    setTitle(titleToken(category));
}

void ShopMenu::searchByPrice() {
    const double min_price = Item::input_price("min price: ");
    const double max_price = Item::input_price("max price: ");
    std::ostringstream item_price;
    item_price << min_price;
    item_price << "-";
    item_price << max_price;
    this->shop_items = this->item_service.queryItemsByPrice(min_price, max_price);
    page_view.item_number = static_cast<int>(shop_items.size());
    page_view.current_page_number = 0;
    page_view.total_pages =  std::ceil(static_cast<double>(page_view.item_number)/page_view.number_per_page);
    this->view_state = 4;
    setTitle(titleToken(item_price.str()));
}

void ShopMenu::cartItem() {
    int item_index = -1;
    const int from = page_view.current_page_number * page_view.number_per_page;
    const int to = (from + page_view.number_per_page > page_view.item_number) ? page_view.item_number : from + page_view.number_per_page;
    FormMenu item_select_menu("Which item do you want to put into cart");
    item_select_menu.addItem(shop_items[from].get_name(),
        [&item_index,from]() {
            item_index = from;
        });
    if (to-1!=from) {
        item_select_menu.addItem(shop_items[to-1].get_name(),
           [&item_index, to]() {
               item_index = to-1;
           });
    }
    item_select_menu.addItem("Cancel",
        []() {});
    clearScreen();
    this->showPage();
    item_select_menu.run();

    if (item_index==-1) {
        return;
    }

    // todo: notice the quantity should not over its stock
    if (const int item_id = shop_items[item_index].get_id();this->cart.checkItem(item_id)) {
        FormMenu ackMenu("This item is already in cart");
        ackMenu.addItem("Add quantity",
            [this,item_index]() {
                const int addition = Cart::inputItemNumber("Enter quantity: ");
                const int quantity = this->cart.getCartItemQuantity(item_index);
                this->cart.updateCartItem(item_index,quantity + addition);
            });
        ackMenu.addItem("Cancel",[]() {});
        ackMenu.run();
    }
    else if (const int quantity = Cart::inputItemNumber("Quantity: "); quantity > 0) {
        this->cart.addCartItem(shop_items[item_index],quantity);
    }
    else {
        std::cout << "Quantity is zero, nothing change in cart";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void ShopMenu::updateCartItem() {
    if (this->cart.isCartEmpty()) {
        std::cout << "My cart is Empty";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    const int index = this->cart.inputItemIndex();
    if (const int quantity =
        Cart::inputItemNumber("Updated quantity[ "
            + std::to_string(this->cart.getCartItemQuantity(index)) + " -> ? ]: "); quantity == 0) {
        FormMenu ackMenu("You have input 0 in quantity");
        ackMenu.addItem("Delete this item from cart",
            [this,index]() {
                this->cart.removeCartItem(index);
            });
        ackMenu.addItem("Cancelled",
            [this]() {
                std::cout << "Nothing changed" << std::endl;
            });
        ackMenu.run();
    }
    else {
        this->cart.updateCartItem(index, quantity);
        std::cout << "Item quantity updated" << std::endl;
    }
}

void ShopMenu::deleteCartItem() {
    if (this->cart.isCartEmpty()) {
        std::cout << "My cart is Empty";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    FormMenu delete_menu("Which way do you want to delete items");
    delete_menu.addItem("Delete one item from cart",
        [this]() {
            const int index = this->cart.inputItemIndex();
            this->cart.removeCartItem(index);
            std::cout << "Item removed from cart" << std::endl;
        });
    delete_menu.addItem("Multi-Select Delete",
        [this]() {
            const std::string input = FormMenu::getStrInput("input indexes separated by space:");
            std::istringstream iss(input);
            int num;
            std::set<int> id_set;
            while (iss>>num) {
                if (this->cart.isInList(num-1))
                id_set.insert(this->cart.get_cart_list().itemId_vector[num-1]);
            }

            for (const int &id : id_set) {
                this->cart.removeCartItemById(id);
            }

            std::cout << id_set.size() <<" items removed from cart" << std::endl;
        });
    delete_menu.addItem("Delete all items from cart",
        [this]() {
            this->cart.removeAllCartItem();
            std::cout <<  "All items removed" << std::endl;
        });
    delete_menu.addItem("Cancel",[](){});
    this->cart.showCart();
    delete_menu.run();
}

void ShopMenu::showCartMenu() {
    Menu cart_menu("My cart");
    cart_menu.addItem("Show my cart",
        [this]() {
            if (this->cart.isCartEmpty()) {
                std::cout << "My cart is Empty";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else
                this->cart.showCart();
        });
    cart_menu.addItem("Update my cart",
        [this]() {
            this->updateCartItem();
        });
    cart_menu.addItem("Delete cart",
        [this]() {
            this->deleteCartItem();
    });
    cart_menu.run();
}

void ShopMenu::updateCart() const {
    if (!item_service.updateCart(this->cart)) {
        throw std::runtime_error("Failed to update cart");
    }
}

