#include "storage_hashmap.h"
#include <algorithm>

void HashMapStorage::insert(const Transaction& t) {
    byTid[t.transaction_id] = t;
    byCustomer[t.customer_id].push_back(t.transaction_id);
    byProduct[t.product_id].push_back(t.transaction_id);
}

std::vector<Transaction> HashMapStorage::searchByCustomer(const std::string& cid) const {
    std::vector<Transaction> result;
    auto it = byCustomer.find(cid);
    if (it == byCustomer.end()) return result;
    for (const auto& tid : it->second) {
        auto jt = byTid.find(tid);
        if (jt != byTid.end()) result.push_back(jt->second);
    }
    return result;
}

std::vector<Transaction> HashMapStorage::searchByProduct(const std::string& pid) const {
    std::vector<Transaction> result;
    auto it = byProduct.find(pid);
    if (it == byProduct.end()) return result;
    for (const auto& tid : it->second) {
        auto jt = byTid.find(tid);
        if (jt != byTid.end()) result.push_back(jt->second);
    }
    return result;
}

bool HashMapStorage::update(const std::string& tid, const Transaction& updated) {
    auto it = byTid.find(tid);
    if (it == byTid.end()) return false;

    const Transaction& old = it->second;

    // Hapus entri lama dari indeks customer
    auto& cVec = byCustomer[old.customer_id];
    cVec.erase(std::remove(cVec.begin(), cVec.end(), tid), cVec.end());

    // Hapus entri lama dari indeks product
    auto& pVec = byProduct[old.product_id];
    pVec.erase(std::remove(pVec.begin(), pVec.end(), tid), pVec.end());

    // Perbarui data utama (byTid)
    it->second = updated;

    // Daftarkan ulang ke indeks baru
    byCustomer[updated.customer_id].push_back(tid);
    byProduct[updated.product_id].push_back(tid);

    return true;
}

bool HashMapStorage::remove(const std::string& tid) {
    auto it = byTid.find(tid);
    if (it == byTid.end()) return false;

    const Transaction& t = it->second;

    // Hapus dari indeks customer
    auto& cVec = byCustomer[t.customer_id];
    cVec.erase(std::remove(cVec.begin(), cVec.end(), tid), cVec.end());

    // Hapus dari indeks product
    auto& pVec = byProduct[t.product_id];
    pVec.erase(std::remove(pVec.begin(), pVec.end(), tid), pVec.end());

    byTid.erase(it);
    return true;
}

size_t HashMapStorage::memoryUsage() const {
    size_t mem = 0;

    // byTid
    for (const auto& kv : byTid)
        mem += kv.first.size() + sizeof(Transaction);

    // byCustomer
    for (const auto& kv : byCustomer) {
        mem += kv.first.size();
        for (const auto& s : kv.second) mem += s.size() + sizeof(std::string);
    }

    // byProduct
    for (const auto& kv : byProduct) {
        mem += kv.first.size();
        for (const auto& s : kv.second) mem += s.size() + sizeof(std::string);
    }

    // perkiraan overhead bucket array dari ketiga map
    mem += byTid.bucket_count() * sizeof(void*);
    mem += byCustomer.bucket_count() * sizeof(void*);
    mem += byProduct.bucket_count() * sizeof(void*);

    return mem;
}
