#ifndef STORAGE_VECTOR_H
#define STORAGE_VECTOR_H

#include <vector>
#include <string>
#include "transaction.h"

class VectorStorage {
    std::vector<Transaction> data;

public:
    void insert(const Transaction& t);
    std::vector<Transaction> searchByCustomer(const std::string& cid) const;
    std::vector<Transaction> searchByProduct(const std::string& pid) const;
    bool update(const std::string& tid, const Transaction& updated);
    bool remove(const std::string& tid);
    size_t memoryUsage() const;

    // Akses seluruh data (untuk recommender / benchmark)
    const std::vector<Transaction>& getAll() const { return data; }
    size_t size() const { return data.size(); }
    void clear() { data.clear(); }
};

#endif // STORAGE_VECTOR_H
