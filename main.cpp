#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <iomanip>

#include "transaction.h"
#include "data_loader.h"
#include "storage_vector.h"
#include "storage_hashmap.h"
#include "recommender.h"
#include "benchmark.h"
#include "utils.h"

// ─── Fungsi Pembantu ──────────────────────────────────────────────────────

static void printTransaction(const Transaction& t) {
    printf("  [%s] Cust:%-6s Prod:%-6s %-30s Cat:%-15s Qty:%d Date:%s\n",
        t.transaction_id.c_str(),
        t.customer_id.c_str(),
        t.product_id.c_str(),
        t.product_name.c_str(),
        t.category.c_str(),
        t.quantity,
        t.order_date.c_str());
}

static void pauseMenu() {
    printf("\nTekan Enter untuk kembali ke menu...");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

static int readInt(const std::string& prompt) {
    int v;
    while (true) {
        printf("%s", prompt.c_str());
        if (std::cin >> v) { std::cin.ignore(); return v; }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printf("  Input tidak valid, coba lagi.\n");
    }
}

static std::string readLine(const std::string& prompt) {
    printf("%s", prompt.c_str());
    std::string s;
    std::getline(std::cin, s);
    return trim(s);
}

// ─── Menu CRUD ────────────────────────────────────────────────────────────

static void crudVectorMenu(VectorStorage& vs) {
    int choice = 0;
    do {
        printf("\n=== Vector CRUD ===\n");
        printf("  1. Tampilkan semua transaksi\n");
        printf("  2. Search by Customer ID\n");
        printf("  3. Search by Product ID\n");
        printf("  4. Insert transaksi baru\n");
        printf("  5. Update transaksi\n");
        printf("  6. Delete transaksi\n");
        printf("  7. Ukuran & memory usage\n");
        printf("  0. Kembali\n");
        choice = readInt("Pilih: ");

        if (choice == 1) {
            const auto& data = vs.getAll();
            if (data.empty()) { printf("  (kosong)\n"); }
            else {
                int shown = 0;
                for (const auto& t : data) {
                    printTransaction(t);
                    if (++shown >= 20) {
                        printf("  ... (menampilkan 20 dari %zu)\n", data.size());
                        break;
                    }
                }
            }
            pauseMenu();
        } else if (choice == 2) {
            std::string cid = readLine("Customer ID: ");
            auto res = vs.searchByCustomer(cid);
            printf("  Ditemukan %zu transaksi:\n", res.size());
            for (const auto& t : res) printTransaction(t);
            pauseMenu();
        } else if (choice == 3) {
            std::string pid = readLine("Product ID: ");
            auto res = vs.searchByProduct(pid);
            printf("  Ditemukan %zu transaksi:\n", res.size());
            for (const auto& t : res) printTransaction(t);
            pauseMenu();
        } else if (choice == 4) {
            Transaction t;
            t.transaction_id = readLine("Transaction ID: ");
            t.customer_id    = readLine("Customer ID   : ");
            t.product_id     = readLine("Product ID    : ");
            t.product_name   = readLine("Product Name  : ");
            t.category       = readLine("Category      : ");
            t.quantity       = readInt ("Quantity      : ");
            t.order_date     = readLine("Order Date    : ");
            vs.insert(t);
            printf("  Transaksi berhasil diinsert.\n");
            pauseMenu();
        } else if (choice == 5) {
            std::string tid = readLine("Transaction ID yang diupdate: ");
            Transaction t;
            t.transaction_id = tid;
            t.customer_id    = readLine("Customer ID baru : ");
            t.product_id     = readLine("Product ID baru  : ");
            t.product_name   = readLine("Product Name baru: ");
            t.category       = readLine("Category baru    : ");
            t.quantity       = readInt ("Quantity baru    : ");
            t.order_date     = readLine("Order Date baru  : ");
            printf("  %s\n", vs.update(tid, t) ? "Update berhasil." : "Transaction ID tidak ditemukan.");
            pauseMenu();
        } else if (choice == 6) {
            std::string tid = readLine("Transaction ID yang dihapus: ");
            printf("  %s\n", vs.remove(tid) ? "Delete berhasil." : "Transaction ID tidak ditemukan.");
            pauseMenu();
        } else if (choice == 7) {
            printf("  Jumlah transaksi : %zu\n", vs.size());
            printf("  Estimasi memori  : %zu bytes\n", vs.memoryUsage());
            pauseMenu();
        }
    } while (choice != 0);
}

static void crudHashMapMenu(HashMapStorage& hs) {
    int choice = 0;
    do {
        printf("\n=== HashMap CRUD ===\n");
        printf("  1. Tampilkan semua transaksi\n");
        printf("  2. Search by Customer ID\n");
        printf("  3. Search by Product ID\n");
        printf("  4. Insert transaksi baru\n");
        printf("  5. Update transaksi\n");
        printf("  6. Delete transaksi\n");
        printf("  7. Ukuran & memory usage\n");
        printf("  0. Kembali\n");
        choice = readInt("Pilih: ");

        if (choice == 1) {
            const auto& data = hs.getAll();
            if (data.empty()) { printf("  (kosong)\n"); }
            else {
                int shown = 0;
                for (const auto& [tid, t] : data) {
                    printTransaction(t);
                    if (++shown >= 20) {
                        printf("  ... (menampilkan 20 dari %zu)\n", data.size());
                        break;
                    }
                }
            }
            pauseMenu();
        } else if (choice == 2) {
            std::string cid = readLine("Customer ID: ");
            auto res = hs.searchByCustomer(cid);
            printf("  Ditemukan %zu transaksi:\n", res.size());
            for (const auto& t : res) printTransaction(t);
            pauseMenu();
        } else if (choice == 3) {
            std::string pid = readLine("Product ID: ");
            auto res = hs.searchByProduct(pid);
            printf("  Ditemukan %zu transaksi:\n", res.size());
            for (const auto& t : res) printTransaction(t);
            pauseMenu();
        } else if (choice == 4) {
            Transaction t;
            t.transaction_id = readLine("Transaction ID: ");
            t.customer_id    = readLine("Customer ID   : ");
            t.product_id     = readLine("Product ID    : ");
            t.product_name   = readLine("Product Name  : ");
            t.category       = readLine("Category      : ");
            t.quantity       = readInt ("Quantity      : ");
            t.order_date     = readLine("Order Date    : ");
            hs.insert(t);
            printf("  Transaksi berhasil diinsert.\n");
            pauseMenu();
        } else if (choice == 5) {
            std::string tid = readLine("Transaction ID yang diupdate: ");
            Transaction t;
            t.transaction_id = tid;
            t.customer_id    = readLine("Customer ID baru : ");
            t.product_id     = readLine("Product ID baru  : ");
            t.product_name   = readLine("Product Name baru: ");
            t.category       = readLine("Category baru    : ");
            t.quantity       = readInt ("Quantity baru    : ");
            t.order_date     = readLine("Order Date baru  : ");
            printf("  %s\n", hs.update(tid, t) ? "Update berhasil." : "Transaction ID tidak ditemukan.");
            pauseMenu();
        } else if (choice == 6) {
            std::string tid = readLine("Transaction ID yang dihapus: ");
            printf("  %s\n", hs.remove(tid) ? "Delete berhasil." : "Transaction ID tidak ditemukan.");
            pauseMenu();
        } else if (choice == 7) {
            printf("  Jumlah transaksi : %zu\n", hs.size());
            printf("  Estimasi memori  : %zu bytes\n", hs.memoryUsage());
            pauseMenu();
        }
    } while (choice != 0);
}

// ─── Fungsi Utama ─────────────────────────────────────────────────────────

int main() {
    std::vector<Transaction> allData;
    VectorStorage  vs;
    HashMapStorage hs;

    int choice = 0;
    do {
        printf("\n");
        printSeparator(50);
        printf("  SISTEM REKOMENDASI PRODUK - Kelompok 4.6\n");
        printSeparator(50);
        printf("  1. Load Dataset\n");
        printf("  2. Vector CRUD\n");
        printf("  3. HashMap CRUD\n");
        printf("  4. Top-N Products\n");
        printf("  5. Frequently Bought Together\n");
        printf("  6. Similar Customers\n");
        printf("  7. Run Benchmark\n");
        printf("  8. Exit\n");
        printSeparator(50);
        choice = readInt("Pilih menu: ");

        switch (choice) {
        case 1: {
            std::string folder = readLine("Path folder dataset (default: dataset): ");
            if (folder.empty()) folder = "dataset";
            printf("  Memuat data dari '%s'...\n", folder.c_str());
            allData = loadData(folder);
            printf("  Berhasil memuat %zu transaksi.\n", allData.size());

            // Isi kedua storage dengan data yang dimuat
            vs.clear(); hs.clear();
            for (const auto& t : allData) { vs.insert(t); hs.insert(t); }
            printf("  Data dimuat ke Vector (%zu) dan HashMap (%zu).\n",
                vs.size(), hs.size());
            pauseMenu();
            break;
        }
        case 2:
            crudVectorMenu(vs);
            break;
        case 3:
            crudHashMapMenu(hs);
            break;
        case 4: {
            if (allData.empty()) { printf("  Load dataset terlebih dahulu.\n"); pauseMenu(); break; }
            int n = readInt("Tampilkan Top-N produk (N): ");
            auto res = topNProducts(allData, n);
            printf("\n  %-6s %-12s %-30s %s\n", "Rank", "Product ID", "Product Name", "Jumlah Transaksi");
            printSeparator(60);
            // Buat tabel lookup product_id -> product_name
            std::unordered_map<std::string,std::string> pidToName;
            for (const auto& t : allData) pidToName[t.product_id] = t.product_name;
            int rank = 1;
            for (const auto& [pid, cnt] : res) {
                printf("  %-6d %-12s %-30s %d\n",
                    rank++, pid.c_str(), pidToName[pid].c_str(), cnt);
            }
            pauseMenu();
            break;
        }
        case 5: {
            if (allData.empty()) { printf("  Load dataset terlebih dahulu.\n"); pauseMenu(); break; }
            std::string pid = readLine("Product ID: ");
            int n = readInt("Tampilkan Top-N (N): ");
            auto res = frequentlyBoughtTogether(allData, pid, n);

            // Buat tabel lookup nama produk
            std::unordered_map<std::string,std::string> pidToName;
            for (const auto& t : allData) pidToName[t.product_id] = t.product_name;

            printf("\n  Produk yang sering dibeli bersama '%s':\n", pid.c_str());
            printSeparator(60);
            if (res.empty()) { printf("  (tidak ada data)\n"); }
            int rank = 1;
            for (const auto& [p, cnt] : res) {
                printf("  %d. %-12s %-30s (co-occurrence: %d)\n",
                    rank++, p.c_str(), pidToName[p].c_str(), cnt);
            }
            pauseMenu();
            break;
        }
        case 6: {
            if (allData.empty()) { printf("  Load dataset terlebih dahulu.\n"); pauseMenu(); break; }
            std::string cid = readLine("Customer ID: ");
            int n = readInt("Tampilkan Top-N pelanggan serupa (N): ");
            auto res = similarCustomers(allData, cid, n);
            printf("\n  Pelanggan serupa dengan '%s':\n", cid.c_str());
            printSeparator(40);
            if (res.empty()) { printf("  (tidak ada data atau customer tidak ditemukan)\n"); }
            int rank = 1;
            for (const auto& c : res)
                printf("  %d. Customer ID: %s\n", rank++, c.c_str());
            pauseMenu();
            break;
        }
        case 7:
            runBenchmark(allData);
            pauseMenu();
            break;
        case 8:
            printf("  Keluar. Terima kasih!\n");
            break;
        default:
            printf("  Pilihan tidak valid.\n");
        }
    } while (choice != 8);

    return 0;
}
