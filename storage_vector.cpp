#include "storage_vector.h"
#include <algorithm>

void VectorStorage::insert(const Transaction& t) {
    data.push_back(t);
}

std::vector<Transaction> VectorStorage::searchByCustomer(const std::string& cid) const {
    std::vector<Transaction> result;
    for (const auto& t : data) {
        if (t.customer_id == cid) result.push_back(t);
    }
    return result;
}

std::vector<Transaction> VectorStorage::searchByProduct(const std::string& pid) const {
    std::vector<Transaction> result;
    for (const auto& t : data) {
        if (t.product_id == pid) result.push_back(t);
    }
    return result;
}

bool VectorStorage::update(const std::string& tid, const Transaction& updated) {
    for (auto& t : data) {
        if (t.transaction_id == tid) {
            t = updated;
            return true;
        }
    }
    return false;
}

bool VectorStorage::remove(const std::string& tid) {
    auto it = std::find_if(data.begin(), data.end(),
        [&tid](const Transaction& t) { return t.transaction_id == tid; });
    if (it == data.end()) return false;
    data.erase(it);
    return true;
}

size_t VectorStorage::memoryUsage() const {
    // kapasitas * sizeof(Transaction) + overhead vector
    return data.capacity() * sizeof(Transaction) + sizeof(data);
}
