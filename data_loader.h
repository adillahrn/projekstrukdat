#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <string>
#include <vector>
#include "transaction.h"

// Muat dan gabungkan orders.csv + order_items.csv + products.csv menjadi vector Transaction.
// folder: path ke direktori dataset (contoh: "dataset")
std::vector<Transaction> loadData(const std::string& folder);

#endif // DATA_LOADER_H
