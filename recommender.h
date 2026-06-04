#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include <vector>
#include <string>
#include "transaction.h"

// Top-N products by transaction count
std::vector<std::pair<std::string,int>> topNProducts(
    const std::vector<Transaction>& txns,
    int N
);

// Products frequently bought together with a given product_id
std::vector<std::pair<std::string,int>> frequentlyBoughtTogether(
    const std::vector<Transaction>& txns,
    const std::string& product_id,
    int N
);

// Customers with similar purchase category profiles
std::vector<std::string> similarCustomers(
    const std::vector<Transaction>& txns,
    const std::string& customer_id,
    int N
);

#endif // RECOMMENDER_H
