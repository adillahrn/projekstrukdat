#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

struct Transaction{
    int transaction_id;
    string customer_id;
    string product_id;
    string product_name;
    string category;
    int quantity;
    string date;
};

vector<Transaction> transactions;

// index untuk mempercepat pencarian
unordered_map<string, vector<int>> customerIndex;
unordered_map<string, vector<int>> productIndex;


// INSERT TRANSACTION
void insertTransaction(Transaction t){

    transactions.push_back(t);

    int index = transactions.size() - 1;

    customerIndex[t.customer_id].push_back(index);
    productIndex[t.product_id].push_back(index);
}


// SEARCH BY CUSTOMER
void searchByCustomer(string customer){

    if(customerIndex.find(customer) == customerIndex.end()){
        cout << "Customer tidak ditemukan\n";
        return;
    }

    for(int idx : customerIndex[customer]){

        Transaction t = transactions[idx];

        cout << t.transaction_id << " "
             << t.product_name << " "
             << t.quantity << endl;
    }
}


// LOAD DATASET CSV
void loadCSV(string filename){

    ifstream file(filename);
    string line;

    getline(file,line); // skip header

    while(getline(file,line)){

        stringstream ss(line);
        string data;

        Transaction t;

        getline(ss,data,',');
        t.transaction_id = stoi(data);

        getline(ss,t.customer_id,',');

        getline(ss,t.product_id,',');

        getline(ss,t.product_name,',');

        getline(ss,t.category,',');

        getline(ss,data,',');
        t.quantity = stoi(data);

        getline(ss,t.date,',');

        insertTransaction(t);
    }
}


int main(){

    // ukur waktu insert dataset
    auto start = chrono::high_resolution_clock::now();

    loadCSV("transactions.csv");

    auto stop = chrono::high_resolution_clock::now();

    auto insertTime =
    chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "Insert Time : "
         << insertTime.count()
         << " ms\n";


    // uji search
    start = chrono::high_resolution_clock::now();

    searchByCustomer("C001");

    stop = chrono::high_resolution_clock::now();

    auto searchTime =
    chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "Search Time : "
         << searchTime.count()
         << " microseconds\n";
}