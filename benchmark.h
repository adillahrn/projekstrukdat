#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vector>
#include <string>
#include <chrono>
#include <functional>
#include "transaction.h"
#include "storage_vector.h"
#include "storage_hashmap.h"

// Ukur waktu eksekusi suatu fungsi dalam milidetik (dirata-rata dari 'runs' pengulangan)
template<typename Func>
double measureTimeMs(Func f, int runs = 5) {
    double total = 0.0;
    for (int i = 0; i < runs; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        f();
        auto end = std::chrono::high_resolution_clock::now();
        total += std::chrono::duration<double, std::milli>(end - start).count();
    }
    return total / runs;
}

// Estimasi penggunaan memori (bytes) untuk n transaksi berdasarkan tipe storage
size_t estimateMemory(size_t n, bool isHashMap);

// Jalankan semua benchmark dan cetak hasilnya dalam bentuk tabel
void runBenchmark(const std::vector<Transaction>& allData);

#endif // BENCHMARK_H
