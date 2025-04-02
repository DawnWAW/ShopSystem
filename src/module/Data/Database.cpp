//
// Created by dawn on 2025/4/2.
//

#include "Database.h"

#include <iostream>
#include <ostream>

Database::Database() {
    int rc = sqlite3_open("test.db", &this->db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database"<< sqlite3_errmsg(this->db) << std::endl;
    }
    else {
        std::cout << "Database opened" << std::endl;
    }
}
