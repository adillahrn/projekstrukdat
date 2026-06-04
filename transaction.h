#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

struct Transaction {
    std::string transaction_id;
    std::string customer_id;
    std::string product_id;
    std::string product_name;
    std::string category;
    int quantity;
    std::string order_date;
};

#endif // TRANSACTION_H
