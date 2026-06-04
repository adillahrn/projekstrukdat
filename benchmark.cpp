#include "benchmark.h"
#include <cstdio>
#include <algorithm>

static const int RUNS = 5;
static const int SIZES[] = {1000, 5000, 10000, 20000};
static const int NUM_SIZES = 4;

size_t estimateMemory(size_t n, bool isHashMap) {
    if (!isHashMap) {
        // vektor: n * sizeof(Transaction) + sedikit overhead
        return n * sizeof(Transaction) + sizeof(std::vector<Transaction>);
    } else {
        // HashMap: map utama + dua map indeks; estimasi kasar per entri
        size_t perEntry = sizeof(Transaction)          // nilai byTid
                        + 3 * sizeof(std::string)      // kunci di 3 map
                        + 2 * sizeof(std::string);     // tid tersimpan di 2 vektor indeks
        return n * perEntry + 3 * 64 * sizeof(void*);  // + estimasi overhead bucket
    }
}

// Fungsi pembantu: buat sub-dataset sebanyak 'n' entri (berulang jika data kurang)
static std::vector<Transaction> makeDataset(const std::vector<Transaction>& src, size_t n) {
    std::vector<Transaction> ds;
    ds.reserve(n);
    for (size_t i = 0; i < n; i++)
        ds.push_back(src[i % src.size()]);
    // Beri setiap entri transaction_id unik untuk menghindari duplikasi kunci
    for (size_t i = 0; i < ds.size(); i++)
        ds[i].transaction_id = std::to_string(i + 1);
    return ds;
}

void runBenchmark(const std::vector<Transaction>& allData) {
    if (allData.empty()) {
        printf("  [!] Dataset kosong. Load dataset terlebih dahulu.\n");
        return;
    }

    // --- Header tabel ---
    printf("\n");
    printf("%-20s %-6s | %-12s %-12s | %-12s %-12s | %-12s %-12s | %-12s %-12s | %-12s %-12s\n",
        "Operasi", "N",
        "Vec(ms)", "Map(ms)",
        "Vec(ms)", "Map(ms)",
        "Vec(ms)", "Map(ms)",
        "Vec(ms)", "Map(ms)",
        "Vec(ms)", "Map(ms)");
    printf("%-20s %-6s   %-12s %-12s   %-12s %-12s   %-12s %-12s   %-12s %-12s   %-12s %-12s\n",
        "", "",
        "[Insert]", "[Insert]",
        "[SrchCust]","[SrchCust]",
        "[SrchProd]","[SrchProd]",
        "[Update]","[Update]",
        "[Delete]","[Delete]");
    printf("%s\n", std::string(140, '-').c_str());

    for (int si = 0; si < NUM_SIZES; si++) {
        int n = SIZES[si];

        // Pastikan tidak melebihi data yang tersedia (dengan pengulangan siklis)
        std::vector<Transaction> ds = makeDataset(allData, (size_t)n);

        // Ambil sampel customer_id dan product_id dari tengah dataset untuk operasi lainnya
        std::string sampleCid = ds[n / 2].customer_id;
        std::string samplePid = ds[n / 2].product_id;
        std::string sampleTid = ds[n / 2].transaction_id;
        Transaction updatedTxn = ds[n / 2];
        updatedTxn.quantity = 99;

        // ---- INSERT ----
        double vecInsert = measureTimeMs([&]() {
            VectorStorage vs;
            for (const auto& t : ds) vs.insert(t);
        }, RUNS);

        double mapInsert = measureTimeMs([&]() {
            HashMapStorage hs;
            for (const auto& t : ds) hs.insert(t);
        }, RUNS);

        // Isi storage sekali di luar bagian terukur untuk operasi berikutnya
        VectorStorage vs;
        HashMapStorage hs;
        for (const auto& t : ds) { vs.insert(t); hs.insert(t); }

        // ---- SEARCH BY CUSTOMER ----
        double vecSrchCust = measureTimeMs([&]() {
            volatile auto r = vs.searchByCustomer(sampleCid);
            (void)r;
        }, RUNS);
        double mapSrchCust = measureTimeMs([&]() {
            volatile auto r = hs.searchByCustomer(sampleCid);
            (void)r;
        }, RUNS);

        // ---- SEARCH BY PRODUCT ----
        double vecSrchProd = measureTimeMs([&]() {
            volatile auto r = vs.searchByProduct(samplePid);
            (void)r;
        }, RUNS);
        double mapSrchProd = measureTimeMs([&]() {
            volatile auto r = hs.searchByProduct(samplePid);
            (void)r;
        }, RUNS);

        // ---- UPDATE ----
        double vecUpdate = measureTimeMs([&]() {
            vs.update(sampleTid, updatedTxn);
        }, RUNS);
        double mapUpdate = measureTimeMs([&]() {
            hs.update(sampleTid, updatedTxn);
        }, RUNS);

        // ---- DELETE ----
        // Setiap run menyisipkan ulang lalu menghapus agar elemen selalu ada
        double vecDelete = measureTimeMs([&]() {
            vs.insert(updatedTxn);
            vs.remove(updatedTxn.transaction_id);
        }, RUNS);
        double mapDelete = measureTimeMs([&]() {
            hs.insert(updatedTxn);
            hs.remove(updatedTxn.transaction_id);
        }, RUNS);

        printf("%-20s %-6d | %-12.4f %-12.4f | %-12.4f %-12.4f | %-12.4f %-12.4f | %-12.4f %-12.4f | %-12.4f %-12.4f\n",
            "All Ops", n,
            vecInsert,  mapInsert,
            vecSrchCust,mapSrchCust,
            vecSrchProd,mapSrchProd,
            vecUpdate,  mapUpdate,
            vecDelete,  mapDelete);
    }

    printf("%s\n", std::string(140, '-').c_str());

    // --- Tabel penggunaan memori ---
    printf("\n%-20s | %-18s | %-18s\n", "N", "Vector Memory (B)", "HashMap Memory (B)");
    printf("%s\n", std::string(62, '-').c_str());
    for (int si = 0; si < NUM_SIZES; si++) {
        int n = SIZES[si];
        printf("%-20d | %-18zu | %-18zu\n",
            n,
            estimateMemory(n, false),
            estimateMemory(n, true));
    }
    printf("%s\n", std::string(62, '-').c_str());
    printf("\n  Waktu dalam milidetik (rata-rata %d runs per operasi).\n\n", RUNS);
}
