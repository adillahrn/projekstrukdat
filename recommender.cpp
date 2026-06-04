#include "recommender.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

// --- Top-N Produk ---
// Hitung kemunculan setiap product_id di seluruh transaksi.
std::vector<std::pair<std::string,int>> topNProducts(
    const std::vector<Transaction>& txns,
    int N)
{
    std::unordered_map<std::string,int> freq;
    for (const auto& t : txns)
        freq[t.product_id]++;

    std::vector<std::pair<std::string,int>> sorted(freq.begin(), freq.end());
    std::sort(sorted.begin(), sorted.end(),
        [](const auto& a, const auto& b){ return a.second > b.second; });

    if (N > 0 && (int)sorted.size() > N)
        sorted.resize(N);
    return sorted;
}

// --- Sering Dibeli Bersama ---
// Transaksi dikelompokkan berdasarkan (customer_id + order_date) sebagai proxy "satu order".
// Produk yang muncul dalam keranjang yang sama dihitung frekuensinya.
std::vector<std::pair<std::string,int>> frequentlyBoughtTogether(
    const std::vector<Transaction>& txns,
    const std::string& product_id,
    int N)
{
    // Kelompokkan berdasarkan (customer_id + order_date) sebagai "keranjang belanja"
    std::unordered_map<std::string, std::vector<std::string>> baskets;
    for (const auto& t : txns) {
        std::string key = t.customer_id + "|" + t.order_date;
        baskets[key].push_back(t.product_id);
    }

    // Untuk setiap keranjang yang mengandung produk target, hitung produk yang muncul bersama
    std::unordered_map<std::string,int> coFreq;
    for (const auto& [key, products] : baskets) {
        bool found = false;
        for (const auto& p : products)
            if (p == product_id) { found = true; break; }
        if (!found) continue;

        for (const auto& p : products) {
            if (p != product_id) coFreq[p]++;
        }
    }

    std::vector<std::pair<std::string,int>> result(coFreq.begin(), coFreq.end());
    std::sort(result.begin(), result.end(),
        [](const auto& a, const auto& b){ return a.second > b.second; });

    if (N > 0 && (int)result.size() > N)
        result.resize(N);
    return result;
}

// --- Pelanggan Serupa ---
// Setiap pelanggan direpresentasikan sebagai himpunan kategori yang pernah dibeli.
// Kemiripan diukur menggunakan Jaccard: |irisan| / |gabungan|.
std::vector<std::string> similarCustomers(
    const std::vector<Transaction>& txns,
    const std::string& customer_id,
    int N)
{
    // Bangun himpunan kategori per pelanggan
    std::unordered_map<std::string, std::unordered_set<std::string>> customerCats;
    for (const auto& t : txns)
        customerCats[t.customer_id].insert(t.category);

    auto it = customerCats.find(customer_id);
    if (it == customerCats.end()) return {};

    const auto& targetCats = it->second;

    std::vector<std::pair<std::string,double>> scores;
    for (const auto& [cid, cats] : customerCats) {
        if (cid == customer_id) continue;

        // Hitung kemiripan Jaccard antara pelanggan target dan pelanggan lain
        int intersect = 0;
        for (const auto& cat : cats)
            if (targetCats.count(cat)) intersect++;
        int unionSize = (int)targetCats.size() + (int)cats.size() - intersect;
        double jaccard = (unionSize > 0) ? (double)intersect / unionSize : 0.0;

        scores.push_back({cid, jaccard});
    }

    std::sort(scores.begin(), scores.end(),
        [](const auto& a, const auto& b){ return a.second > b.second; });

    std::vector<std::string> result;
    int count = (N > 0) ? std::min(N, (int)scores.size()) : (int)scores.size();
    for (int i = 0; i < count; i++)
        result.push_back(scores[i].first);
    return result;
}
