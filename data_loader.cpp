#include "data_loader.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

// orders.csv: order_id, order_date, customer_id, total_price
// order_items.csv: order_item_id, order_id, product_id, quantity, price_at_purchase
// products.csv: product_id, product_name, category, price, supplier_id

std::vector<Transaction> loadData(const std::string& folder) {
    // --- Muat data produk ---
    // kunci: product_id -> {product_name, category}
    std::unordered_map<std::string, std::pair<std::string,std::string>> products;
    {
        std::ifstream f(folder + "/products.csv");
        if (!f.is_open()) {
            std::cerr << "[ERROR] Cannot open products.csv\n";
        } else {
            std::string line;
            std::getline(f, line); // lewati baris header
            while (std::getline(f, line)) {
                if (line.empty()) continue;
                auto cols = splitCSV(line);
                if (cols.size() < 3) continue;
                products[cols[0]] = {cols[1], cols[2]};
            }
        }
    }

    // --- Muat data orders ---
    // kunci: order_id -> {order_date, customer_id}
    std::unordered_map<std::string, std::pair<std::string,std::string>> orders;
    {
        std::ifstream f(folder + "/orders.csv");
        if (!f.is_open()) {
            std::cerr << "[ERROR] Cannot open orders.csv\n";
        } else {
            std::string line;
            std::getline(f, line); // lewati baris header
            while (std::getline(f, line)) {
                if (line.empty()) continue;
                auto cols = splitCSV(line);
                if (cols.size() < 3) continue;
                // order_id, order_date, customer_id, total_price
                orders[cols[0]] = {cols[1], cols[2]};
            }
        }
    }

    // --- Muat order_items dan gabungkan dengan orders & products ---
    std::vector<Transaction> result;
    {
        std::ifstream f(folder + "/order_items.csv");
        if (!f.is_open()) {
            std::cerr << "[ERROR] Cannot open order_items.csv\n";
        } else {
            std::string line;
            std::getline(f, line); // lewati baris header
            while (std::getline(f, line)) {
                if (line.empty()) continue;
                // order_item_id, order_id, product_id, quantity, price_at_purchase
                auto cols = splitCSV(line);
                if (cols.size() < 5) continue;

                std::string item_id   = cols[0];
                std::string order_id  = cols[1];
                std::string prod_id   = cols[2];
                std::string qty_str   = cols[3];

                // cari info order berdasarkan order_id
                auto oit = orders.find(order_id);
                if (oit == orders.end()) continue;
                const std::string& order_date  = oit->second.first;
                const std::string& customer_id = oit->second.second;

                // cari info produk berdasarkan product_id
                std::string prod_name = "Unknown";
                std::string category  = "Unknown";
                auto pit = products.find(prod_id);
                if (pit != products.end()) {
                    prod_name = pit->second.first;
                    category  = pit->second.second;
                }

                int qty = 0;
                try { qty = std::stoi(qty_str); } catch (...) {}

                Transaction t;
                t.transaction_id = item_id;
                t.customer_id    = customer_id;
                t.product_id     = prod_id;
                t.product_name   = prod_name;
                t.category       = category;
                t.quantity       = qty;
                t.order_date     = order_date;

                result.push_back(std::move(t));
            }
        }
    }

    return result;
}
