#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;

struct Transaction {
    string transaction_id;
    string customer_id;
    string product_id;
    string product_name;
    string category;
    int quantity;
    string order_date;
};

/* LOAD CSV */

vector<Transaction> loadCSV(const string& filename) {

    vector<Transaction> data;

    ifstream file(filename);
    string line;

    getline(file, line); // skip header

    while (getline(file, line)) {

        stringstream ss(line);
        string token;

        Transaction t;

        getline(ss, t.transaction_id, ',');
        getline(ss, t.customer_id, ',');
        getline(ss, t.product_id, ',');
        getline(ss, t.product_name, ',');
        getline(ss, t.category, ',');
        getline(ss, token, ',');
        t.quantity = stoi(token);
        getline(ss, t.order_date, ',');

        data.push_back(t);
    }

    return data;
}


/* BENCHMARK FUNCTION */

void benchmark(const string& csvFile) {

    vector<Transaction> transactions = loadCSV(csvFile);

    int n = transactions.size();

    if (n == 0) {
        cout << "File kosong\n";
        return;
    }

    /* target data terakhir (worst case) */

    string targetId = transactions[n-1].transaction_id;

    Transaction updatedT = transactions[n-1];
    updatedT.product_name = "UPDATED_PRODUCT";


    /* INSERT */

    Transaction newT;

    newT.transaction_id = "TEST999";
    newT.customer_id = "CUST999";
    newT.product_id = "PROD999";
    newT.product_name = "TestProduct";
    newT.category = "Test";
    newT.quantity = 1;
    newT.order_date = "2026-01-01";

    auto t1 = chrono::high_resolution_clock::now();

    transactions.push_back(newT);

    auto t2 = chrono::high_resolution_clock::now();

    double insertMs =
    chrono::duration<double, milli>(t2 - t1).count();


    /* SEARCH */

    auto t3 = chrono::high_resolution_clock::now();

    for (auto it = transactions.begin(); it != transactions.end(); ++it) {

        if (it->transaction_id == targetId) {
            break;
        }
    }

    auto t4 = chrono::high_resolution_clock::now();

    double searchMs =
    chrono::duration<double, milli>(t4 - t3).count();


    /* UPDATE */

    auto t5 = chrono::high_resolution_clock::now();

    for (auto it = transactions.begin(); it != transactions.end(); ++it) {

        if (it->transaction_id == targetId) {

            *it = updatedT;
            break;
        }
    }

    auto t6 = chrono::high_resolution_clock::now();

    double updateMs =
    chrono::duration<double, milli>(t6 - t5).count();


    /* DELETE */

    auto t7 = chrono::high_resolution_clock::now();

    for (auto it = transactions.begin(); it != transactions.end(); ++it) {

        if (it->transaction_id == targetId) {

            transactions.erase(it);
            break;
        }
    }

    auto t8 = chrono::high_resolution_clock::now();

    double deleteMs =
    chrono::duration<double, milli>(t8 - t7).count();


    cout << fixed << setprecision(4);

    cout << "| " << setw(12) << n
         << " | " << setw(10) << insertMs
         << " | " << setw(10) << searchMs
         << " | " << setw(10) << updateMs
         << " | " << setw(10) << deleteMs
         << " |" << endl;
}



int main() {

    cout << "\n=== BENCHMARK VECTOR (TRANSACTION DATASET) ===\n";

    cout << "+--------------+------------+------------+------------+------------+\n";
    cout << "| Jumlah Data  | Insert(ms) | Search(ms) | Update(ms) | Delete(ms) |\n";
    cout << "+--------------+------------+------------+------------+------------+\n";

    benchmark("trans1000.csv");
    benchmark("trans10000.csv");
    benchmark("trans20000.csv");

    cout << "+--------------+------------+------------+------------+------------+\n";

    cout << "\nCatatan: Search, Update, Delete menggunakan transaction_id terakhir (worst case).\n";

}