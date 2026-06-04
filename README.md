# Tugas Proyek Struktur Data Semester 4

**Kelompok:** 4.6  
**Anggota:**  
- Adillah Ridwan [M0403241072]
- Tesalonika Natalie Sofi Siregar [M0403241185]  
- Nadya Shafwah Rizalti	[M0403241007]  
- Muhammad Rezonaldo Yunus [M0403241122]

---
# Topik 6: Sistem Rekomendasi Produk Berbasis Riwayat Transaksi

**Konteks industri:** e-commerce

Kasus ini menggambarkan sistem rekomendasi sederhana pada platform e-commerce yang memanfaatkan riwayat transaksi pelanggan. Pada tahap awal, rekomendasi produk dapat dilakukan secara manual atau berbasis aturan sederhana, seperti menampilkan produk terlaris atau produk yang sering dibeli secara bersamaan.

Seiring bertambahnya jumlah transaksi, produk, dan pengguna, pencarian pola pembelian dan penyajian rekomendasi menjadi semakin lambat jika data dikelola secara sederhana. Oleh karena itu, diperlukan pemilihan struktur data yang tepat agar pengelolaan riwayat transaksi dan penyusunan rekomendasi tetap efisien meskipun volume data meningkat.

> Sistem rekomendasi ini **tidak berbasis machine learning**. Rekomendasi dibangun menggunakan aturan sederhana (rule-based) yang dapat dijelaskan secara eksplisit melalui perhitungan frekuensi dan pencarian data.

Bentuk rekomendasi yang dikembangkan dibatasi pada:
- Rekomendasi produk paling sering dibeli (Top-N produk)
- Rekomendasi produk yang sering dibeli bersama dalam transaksi yang sama (frequently bought together)
- Rekomendasi berbasis riwayat pelanggan, yaitu menyarankan produk yang sering muncul pada transaksi pelanggan lain dengan pola pembelian serupa

Seluruh rekomendasi harus dapat dijelaskan secara logis menggunakan data transaksi yang tersedia, tanpa melibatkan model prediktif atau proses pembelajaran otomatis.

---

## Permasalahan di Lapangan
- Riwayat transaksi semakin besar dan sulit dicari secara cepat
- Penyusunan rekomendasi produk menjadi lambat jika data tidak terstruktur dengan baik
- Update data transaksi dan produk memengaruhi performa sistem
- Sistem sederhana tidak mampu menangani pertumbuhan data transaksi

---

## Tujuan Proyek
Membangun sistem rekomendasi produk sederhana berbasis riwayat transaksi, serta membandingkan performa struktur data yang digunakan untuk pengelolaan data transaksi dan penyusunan rekomendasi.

---

## Domain Data (Minimal)
- ID transaksi
- ID pelanggan
- ID produk
- Nama produk
- Kategori produk
- Jumlah pembelian
- Tanggal transaksi

---

## Aktivitas Sistem Wajib
- Insert data transaksi
- Search riwayat transaksi berdasarkan pelanggan atau produk
- Analisis sederhana pola pembelian (misalnya: produk paling sering dibeli, atau produk yang sering muncul dalam transaksi yang sama)
- Generate rekomendasi produk sederhana
- Delete atau update data transaksi

---

## Fokus Analisis
Analisis perbandingan struktur data terhadap:
- Kecepatan pencarian riwayat transaksi
- Efisiensi penyusunan rekomendasi produk berbasis frekuensi
- Kemudahan insert, update, dan delete data transaksi
- Dampak pertumbuhan jumlah transaksi terhadap performa sistem

---

## Target Minggu ke-7 (Progress)
- Spesifikasi sistem & skenario transaksi e-commerce
- Implementasi minimal 1 struktur data
- Dataset awal transaksi (dummy / simulasi)
- Pengujian awal insert dan search transaksi

---

## Target Minggu ke-14 (Final)
- Implementasi lengkap ≥ 2 struktur data
- Grafik perbandingan waktu eksekusi & penggunaan memori
- Demo aplikasi sistem rekomendasi produk sederhana
- Laporan analisis performa & rekomendasi struktur data

---

## Catatan
Mahasiswa **tidak diwajibkan dan tidak dinilai** berdasarkan implementasi:
- Machine learning
- Collaborative filtering
- Content-based filtering
- Similarity matrix atau model prediktif lainnya

Fokus penilaian adalah pada penggunaan **struktur data**, pengelolaan data transaksi, dan analisis performa sistem dalam menghasilkan rekomendasi sederhana berbasis riwayat transaksi. Analisis pola pembelian bersifat sederhana dan berbasis **hitungan frekuensi (counting)**, bukan berbasis prediksi, pembelajaran model, atau perhitungan similarity.


# Product Recommendation System using Vector and Hash Map

## Deskripsi Proyek

Proyek ini merupakan implementasi sistem rekomendasi produk berbasis data transaksi pelanggan menggunakan bahasa C++. Sistem dirancang untuk menganalisis pola pembelian pelanggan dan menghasilkan rekomendasi produk berdasarkan histori transaksi.

Selain fitur rekomendasi, proyek ini juga bertujuan untuk membandingkan performa dua struktur data yang berbeda, yaitu:

* `std::vector`
* `std::unordered_map`

Perbandingan dilakukan melalui serangkaian eksperimen kuantitatif untuk mengevaluasi efisiensi waktu eksekusi dan penggunaan memori pada berbagai operasi data.

---

## Tujuan

1. Mengimplementasikan sistem pengelolaan data transaksi pelanggan.
2. Mengembangkan fitur rekomendasi produk berbasis histori transaksi.
3. Membandingkan performa `std::vector` dan `std::unordered_map`.
4. Menganalisis kompleksitas waktu dan penggunaan memori kedua struktur data.
5. Memberikan rekomendasi struktur data yang paling sesuai untuk sistem skala besar.

---

## Struktur Proyek

```text
project/
├── main.cpp                  # Entry point + menu utama
├── transaction.h             # Struct Transaction
├── data_loader.h
├── data_loader.cpp           # Pembacaan CSV dan penggabungan data
├── storage_vector.h
├── storage_vector.cpp        # Implementasi CRUD menggunakan vector
├── storage_hashmap.h
├── storage_hashmap.cpp       # Implementasi CRUD menggunakan unordered_map
├── recommender.h
├── recommender.cpp           # Algoritma rekomendasi
├── benchmark.h
├── benchmark.cpp             # Pengukuran performa
├── utils.h                   # Fungsi bantuan
└── data/
    ├── orders.csv
    ├── customers.csv
    └── products.csv
```

---

## Struktur Data Utama

### Transaction

Seluruh data transaksi direpresentasikan menggunakan struktur berikut:

```cpp
struct Transaction {
    string transaction_id;
    string customer_id;
    string product_id;
    string product_name;
    string category;
    int quantity;
    string order_date;
};
```

---

## Implementasi Struktur Data

### 1. Vector Storage

Modul ini menyimpan seluruh transaksi menggunakan:

```cpp
vector<Transaction>
```

Implementasi:

```cpp
class VectorStorage {
    vector<Transaction> data;

public:
    void insert(const Transaction& t);
    vector<Transaction> searchByCustomer(const string& cid);
    vector<Transaction> searchByProduct(const string& pid);
    bool update(const string& tid, const Transaction& updated);
    bool remove(const string& tid);
    size_t memoryUsage() const;
};
```

#### Kompleksitas Operasi

| Operasi | Kompleksitas   |
| ------- | -------------- |
| Insert  | O(1) amortized |
| Search  | O(n)           |
| Update  | O(n)           |
| Delete  | O(n)           |

#### Kelebihan

* Implementasi sederhana.
* Penggunaan memori relatif kecil.
* Cocok untuk dataset kecil.

#### Kekurangan

* Pencarian membutuhkan iterasi seluruh data.

---

### 2. HashMap Storage

Modul ini menggunakan:

```cpp
unordered_map<string, Transaction>
```

serta indeks tambahan:

```cpp
unordered_map<string, vector<string>> byCustomer;
unordered_map<string, vector<string>> byProduct;
```

Implementasi:

```cpp
class HashMapStorage {
    unordered_map<string, Transaction> byTid;
    unordered_map<string, vector<string>> byCustomer;
    unordered_map<string, vector<string>> byProduct;

public:
    void insert(const Transaction& t);
    vector<Transaction> searchByCustomer(const string& cid);
    vector<Transaction> searchByProduct(const string& pid);
    bool update(const string& tid, const Transaction& updated);
    bool remove(const string& tid);
    size_t memoryUsage() const;
};
```

#### Kompleksitas Operasi

| Operasi | Kompleksitas Rata-rata |
| ------- | ---------------------- |
| Insert  | O(1)                   |
| Search  | O(1)                   |
| Update  | O(1)                   |
| Delete  | O(1)                   |

#### Kelebihan

* Pencarian sangat cepat.
* Cocok untuk dataset besar.
* Mendukung analisis dan rekomendasi secara efisien.

#### Kekurangan

* Membutuhkan memori lebih besar dibanding vector.

---

## Modul Rekomendasi

### Top-N Produk Terlaris

Menghitung frekuensi pembelian setiap produk menggunakan hash map.

```cpp
vector<pair<string,int>> topNProducts(
    const vector<Transaction>& txns,
    int N
);
```

Output berupa daftar produk dengan jumlah transaksi tertinggi.

---

### Frequently Bought Together

Menentukan produk yang paling sering dibeli bersama suatu produk tertentu.

```cpp
vector<pair<string,int>> frequentlyBoughtTogether(
    const vector<Transaction>& txns,
    const string& product_id,
    int N
);
```

Metode:

1. Kelompokkan transaksi berdasarkan order.
2. Hitung frekuensi pasangan produk.
3. Ambil pasangan dengan frekuensi tertinggi.

Contoh:

```text
Beras
 ├── Telur
 ├── Minyak Goreng
 └── Gula
```

Hash map dipilih karena tujuan utama fitur ini adalah menghitung frekuensi kemunculan pasangan produk secara efisien.

---

### Similar Customers

Mencari pelanggan dengan pola pembelian yang serupa berdasarkan kategori produk yang dibeli.

```cpp
vector<string> similarCustomers(
    const vector<Transaction>& txns,
    const string& customer_id,
    int N
);
```

---

## Benchmark dan Pengukuran Performa

Pengukuran waktu dilakukan menggunakan:

```cpp
template<typename Func>
double measureTimeMs(Func f);
```

berbasis:

```cpp
chrono::high_resolution_clock
```

Estimasi penggunaan memori:

```cpp
size_t estimateMemory(size_t n, bool isHashMap);
```

---

## Skenario Eksperimen

Setiap eksperimen dijalankan:

* 5 kali pengulangan
* Menggunakan rata-rata hasil pengukuran

Ukuran dataset:

* 1.000 transaksi
* 5.000 transaksi
* 10.000 transaksi
* 20.000 transaksi

### Operasi yang Diuji

| Operasi            | Vector | HashMap |
| ------------------ | ------ | ------- |
| Insert Batch       | O(n)   | O(n)    |
| Search by Customer | O(n)   | O(1)    |
| Search by Product  | O(n)   | O(1)    |
| Update             | O(n)   | O(1)    |
| Delete             | O(n)   | O(1)    |
| Top-N Analysis     | O(n)   | O(n)    |

---

## Ekspektasi Hasil

Perbedaan performa paling signifikan diperkirakan muncul pada:

* Search by Customer
* Search by Product
* Update
* Delete

Semakin besar ukuran dataset, semakin besar keuntungan penggunaan `unordered_map`.

---

## Kesimpulan yang Diharapkan

Berdasarkan teori kompleksitas dan hasil benchmark, `std::unordered_map` diperkirakan menjadi struktur data yang paling efisien untuk sistem rekomendasi produk karena mampu melakukan operasi pencarian, pembaruan, dan penghapusan dalam waktu rata-rata O(1).

Sementara itu, `std::vector` digunakan sebagai pembanding untuk menunjukkan pengaruh pemilihan struktur data terhadap performa sistem.
