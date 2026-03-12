#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

// Struct Transaction
struct Transaction{
    string transaction_id;
    string customer_id;
    string product_id;
    string product_name;
    string category;
    int quantity;
    string date;
};

vector<Transaction> transactions;

/* index untuk mempercepat search */
unordered_map<string, vector<int>> customerIndex;
unordered_map<string, vector<int>> productIndex;


// insert transaction
void insertTransaction(Transaction t){

    transactions.push_back(t);

    int index = transactions.size() - 1;

    customerIndex[t.customer_id].push_back(index);
    productIndex[t.product_id].push_back(index);
}


// load dataset
void loadTransactions(string filename){

    ifstream file(filename);
    string line;

    getline(file,line); // skip header

    while(getline(file,line)){

        stringstream ss(line);

        Transaction t;
        string data;

        getline(ss,t.transaction_id,',');
        getline(ss,t.customer_id,',');
        getline(ss,t.product_id,',');
        getline(ss,t.product_name,',');
        getline(ss,t.category,',');

        getline(ss,data,',');
        t.quantity = stoi(data);

        getline(ss,t.date,',');

        insertTransaction(t);
    }

    cout << "Transactions loaded : " << transactions.size() << endl;
}


// search by customer id
void searchByCustomer(string customer){

    if(customerIndex.find(customer) == customerIndex.end()){
        cout << "Customer not found\n";
        return;
    }

    for(int idx : customerIndex[customer]){

        Transaction t = transactions[idx];

        cout << t.transaction_id << " "
             << t.product_name << " "
             << t.quantity << endl;
    }
}


// search by product id
void searchByProduct(string product){

    if(productIndex.find(product) == productIndex.end()){
        cout << "Product not found\n";
        return;
    }

    for(int idx : productIndex[product]){

        Transaction t = transactions[idx];

        cout << t.transaction_id << " "
             << t.customer_id << " "
             << t.quantity << endl;
    }
}


int main(){

    auto start = chrono::high_resolution_clock::now();

    loadTransactions("trans20.csv");

    auto stop = chrono::high_resolution_clock::now();

    auto insertTime =
    chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "\nLoad Transaction Time : "
         << insertTime.count()
         << " ms\n";


    cout << "\nSearch by Customer 9974\n";

    start = chrono::high_resolution_clock::now();

    searchByCustomer("9974");

    stop = chrono::high_resolution_clock::now();

    auto searchTime =
    chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "Search Time : "
         << searchTime.count()
         << " microseconds\n";
}