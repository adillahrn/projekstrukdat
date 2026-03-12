#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace chrono;

struct Transaction {
    string transaction_id;
    string customer_id;
    string product_id;
    string product_name;
    string category;
    int quantity;
    string order_date;
};

vector<Transaction> transactions;

unordered_map<string, vector<int>> customerIndex;
unordered_map<string, vector<int>> productIndex;

void indexTransaction(int idx){
    customerIndex[transactions[idx].customer_id].push_back(idx);
    productIndex[transactions[idx].product_id].push_back(idx);
}

void loadTransactions(string filename){

    auto start = high_resolution_clock::now();

    ifstream file(filename);

    if(!file){
        cout<<"File tidak ditemukan\n";
        return;
    }

    string line;
    getline(file,line);

    while(getline(file,line)){

        stringstream ss(line);
        Transaction t;

        string quantity;

        getline(ss,t.transaction_id,',');
        getline(ss,t.customer_id,',');
        getline(ss,t.product_id,',');
        getline(ss,t.product_name,',');
        getline(ss,t.category,',');
        getline(ss,quantity,',');
        getline(ss,t.order_date,',');

        t.quantity = stoi(quantity);

        transactions.push_back(t);

        indexTransaction(transactions.size()-1);
    }

    auto stop = high_resolution_clock::now();

    cout<<"\nTransactions loaded : "<<transactions.size()<<endl;

    cout<<"Load Time : "
        <<duration_cast<milliseconds>(stop-start).count()
        <<" ms\n";
}

void searchCustomer(string cid){

    auto start = high_resolution_clock::now();

    if(customerIndex.find(cid)==customerIndex.end()){
        cout<<"Customer not found\n";
        return;
    }

    cout<<"\nTransactions for customer "<<cid<<endl;

    for(int idx : customerIndex[cid]){

        auto &t = transactions[idx];

        cout<<t.transaction_id<<" | "
            <<t.product_name<<" | "
            <<t.quantity<<" | "
            <<t.order_date<<endl;
    }

    auto stop = high_resolution_clock::now();

    cout<<"\nSearch Time : "
        <<duration_cast<microseconds>(stop-start).count()
        <<" microseconds\n";
}

void searchProduct(string pid){

    auto start = high_resolution_clock::now();

    if(productIndex.find(pid)==productIndex.end()){
        cout<<"Product not found\n";
        return;
    }

    cout<<"\nTransactions containing product "<<pid<<endl;

    for(int idx : productIndex[pid]){

        auto &t = transactions[idx];

        cout<<t.transaction_id<<" | "
            <<t.customer_id<<" | "
            <<t.quantity<<" | "
            <<t.order_date<<endl;
    }

    auto stop = high_resolution_clock::now();

    cout<<"\nSearch Time : "
        <<duration_cast<microseconds>(stop-start).count()
        <<" microseconds\n";
}

void topProducts(int N){

    unordered_map<string,int> freq;
    unordered_map<string,string> name;

    for(auto &t:transactions){

        freq[t.product_id]+=t.quantity;
        name[t.product_id]=t.product_name;
    }

    vector<pair<string,int>> vec(freq.begin(),freq.end());

    sort(vec.begin(),vec.end(),
    [](auto &a,auto &b){
        return a.second>b.second;
    });

    cout<<"\nTop "<<N<<" Products\n";

    for(int i=0;i<N && i<vec.size();i++){

        cout<<name[vec[i].first]
            <<" ("<<vec[i].first<<") : "
            <<vec[i].second<<endl;
    }
}

void frequentlyBoughtTogether(int N){

    unordered_map<string,vector<string>> trxProducts;

    for(auto &t:transactions){
        trxProducts[t.transaction_id].push_back(t.product_id);
    }

    map<string,int> pairCount;

    for(auto &trx:trxProducts){

        auto &products = trx.second;

        sort(products.begin(),products.end());

        products.erase(unique(products.begin(),products.end()),products.end());

        for(int i=0;i<products.size();i++){
            for(int j=i+1;j<products.size();j++){

                string key = products[i]+"|"+products[j];
                pairCount[key]++;
            }
        }
    }

    vector<pair<string,int>> vec(pairCount.begin(),pairCount.end());

    sort(vec.begin(),vec.end(),
    [](auto &a,auto &b){
        return a.second>b.second;
    });

    cout<<"\nTop "<<N<<" Frequently Bought Together\n";

    for(int i=0;i<N && i<vec.size();i++){

        cout<<vec[i].first
            <<" : "
            <<vec[i].second<<endl;
    }
}

void insertTransaction(){

    Transaction t;

    cout<<"Transaction ID: ";
    cin>>t.transaction_id;

    cout<<"Customer ID: ";
    cin>>t.customer_id;

    cout<<"Product ID: ";
    cin>>t.product_id;

    cout<<"Product Name: ";
    cin>>t.product_name;

    cout<<"Category: ";
    cin>>t.category;

    cout<<"Quantity: ";
    cin>>t.quantity;

    cout<<"Date: ";
    cin>>t.order_date;

    transactions.push_back(t);

    indexTransaction(transactions.size()-1);

    cout<<"Transaction inserted\n";
}

void deleteTransaction(string tid){

    for(int i=0;i<transactions.size();i++){

        if(transactions[i].transaction_id==tid){

            transactions.erase(transactions.begin()+i);

            cout<<"Transaction deleted\n";
            return;
        }
    }

    cout<<"Transaction not found\n";
}

int main(){

    loadTransactions("trans20k.csv");

    int choice;

    while(true){

        cout<<"\n===== MENU =====\n";
        cout<<"1 Search by Customer\n";
        cout<<"2 Search by Product\n";
        cout<<"3 Top Products\n";
        cout<<"4 Frequently Bought Together\n";
        cout<<"5 Insert Transaction\n";
        cout<<"6 Delete Transaction\n";
        cout<<"0 Exit\n";
        cout<<"Choice : ";

        cin>>choice;

        if(choice==0) break;

        if(choice==1){

            string cid;
            cout<<"Customer ID : ";
            cin>>cid;

            searchCustomer(cid);
        }

        else if(choice==2){

            string pid;
            cout<<"Product ID : ";
            cin>>pid;

            searchProduct(pid);
        }

        else if(choice==3){

            topProducts(5);
        }

        else if(choice==4){

            frequentlyBoughtTogether(10);
        }

        else if(choice==5){

            insertTransaction();
        }

        else if(choice==6){

            string tid;
            cout<<"Transaction ID : ";
            cin>>tid;

            deleteTransaction(tid);
        }
    }
}