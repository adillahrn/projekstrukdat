#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

//struct dataset

struct Order{
    string order_id;
    string order_date;
    string customer_id;
};

struct Product{
    string product_id;
    string product_name;
    string category;
};

struct OrderItem{
    string order_id;
    string product_id;
    int quantity;
};

//Struct Transaction
struct Transaction{
    string transaction_id;
    string customer_id;
    string product_id;
    string product_name;
    string category;
    int quantity;
    string date;
};

unordered_map<string, Order> orders;
unordered_map<string, Product> products;
vector<OrderItem> orderItems;

vector<Transaction> transactions;

/* index untuk mempercepat search */
unordered_map<string, vector<int>> customerIndex;
unordered_map<string, vector<int>> productIndex;


//load product

void loadProducts(string filename){

    ifstream file(filename);
    string line;

    getline(file,line); // skip header

    while(getline(file,line)){

        stringstream ss(line);

        Product p;

        getline(ss,p.product_id,',');
        getline(ss,p.product_name,',');
        getline(ss,p.category,',');

        products[p.product_id] = p;
    }

    cout << "Products loaded : " << products.size() << endl;
}


// load orders

void loadOrders(string filename){

    ifstream file(filename);
    string line;

    getline(file,line);

    while(getline(file,line)){

        stringstream ss(line);

        Order o;

        getline(ss,o.order_id,',');
        getline(ss,o.order_date,',');
        getline(ss,o.customer_id,',');

        orders[o.order_id] = o;
    }

    cout << "Orders loaded : " << orders.size() << endl;
}


// load orders item

void loadOrderItems(string filename){

    ifstream file(filename);
    string line;

    getline(file,line);

    while(getline(file,line)){

        stringstream ss(line);

        OrderItem oi;
        string data;

        getline(ss,data,','); // order_item_id
        getline(ss,oi.order_id,',');
        getline(ss,oi.product_id,',');

        getline(ss,data,',');
        oi.quantity = stoi(data);

        orderItems.push_back(oi);
    }

    cout << "Order items loaded : " << orderItems.size() << endl;
}


// insert transaction
void insertTransaction(Transaction t){

    transactions.push_back(t);

    int index = transactions.size() - 1;

    customerIndex[t.customer_id].push_back(index);
    productIndex[t.product_id].push_back(index);
}


//transaction data

void buildTransactions(){

    for(auto &oi : orderItems){

        if(orders.find(oi.order_id) == orders.end()) continue;
        if(products.find(oi.product_id) == products.end()) continue;

        Order o = orders[oi.order_id];
        Product p = products[oi.product_id];

        Transaction t;

        t.transaction_id = o.order_id;
        t.customer_id = o.customer_id;
        t.product_id = p.product_id;
        t.product_name = p.product_name;
        t.category = p.category;
        t.quantity = oi.quantity;
        t.date = o.order_date;

        insertTransaction(t);
    }

    cout << "Transactions built : " << transactions.size() << endl;
}


//search by customer id

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


//search by product id

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

    loadProducts("products.csv");
    loadOrders("orders.csv");
    loadOrderItems("order_items.csv");

    buildTransactions();

    auto stop = chrono::high_resolution_clock::now();

    auto insertTime =
    chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "\nBuild Transaction Time : "
         << insertTime.count()
         << " ms\n";


    cout << "\nSearch by Customer 7318\n";

    start = chrono::high_resolution_clock::now();

    searchByCustomer("9974");

    stop = chrono::high_resolution_clock::now();

    auto searchTime =
    chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "Search Time : "
         << searchTime.count()
         << " ms\n";

}