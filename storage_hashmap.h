#ifndef STORAGE_HASHMAP_H
#define STORAGE_HASHMAP_H

#include <unordered_map>
#include <vector>
#include <string>
#include "transaction.h"

class HashMapStorage {
    std::unordered_map<std::string, Transaction>              byTid;
    std::unordered_map<std::string, std::vector<std::string>> byCustomer;
    std::unordered_map<std::string, std::vector<std::string>> byProduct;

public:
    void insert(const Transaction& t);
    std::vector<Transaction> searchByCustomer(const std::string& cid) const;
    std::vector<Transaction> searchByProduct(const std::string& pid) const;
    bool update(const std::string& tid, const Transaction& updated);
    bool remove(const std::string& tid);
    size_t memoryUsage() const;

    // Akses seluruh data (untuk recommender / benchmark)
    const std::unordered_map<std::string, Transaction>& getAll() const { return byTid; }
    size_t size() const { return byTid.size(); }
    void clear() { byTid.clear(); byCustomer.clear(); byProduct.clear(); }
};

#endif // STORAGE_HASHMAP_H
