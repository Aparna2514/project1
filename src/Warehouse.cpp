#include "../include/Warehouse.h"
#include "../include/Utility.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
using namespace std;

Warehouse::Warehouse() {}

Warehouse::Warehouse(string id, string address, string pin, string stat)
    : warehouseID(id), exactAddress(address), pincode(pin), status(stat) {}

string Warehouse::getWarehouseFileName() const {
    return "data/warehouse_" + warehouseID + ".txt";
}

void Warehouse::registerWarehouse() {
    Utility u;
    int idNum = u.generateID();
    warehouseID = "W" + to_string(idNum);
    
    cout << "Enter Exact Address: ";
    getline(cin >> ws, exactAddress);

    cout << "Enter Pincode: ";
    cin >> pincode;

    status = "Active";

    ofstream out("data/master_warehouse.txt", ios::app);
    out << warehouseID << "," << exactAddress << "," << pincode << "," << status << "\n";
    out.close();

    ofstream inv(getWarehouseFileName());  // creates empty inventory file
    inv.close();

    cout << "✅ Warehouse registered with ID: " << warehouseID << endl;
}

void Warehouse::deleteWarehouse(const string& id) {
    ifstream in("data/master_warehouse.txt");
    ofstream temp("data/temp.txt");
    string line;
    bool found = false;

    while (getline(in, line)) {
        if (line.find(id) != 0) {
            temp << line << "\n";
        } else {
            found = true;
        }
    }
    in.close();
    temp.close();

    remove("data/master_warehouse.txt");
    rename("data/temp.txt", "data/master_warehouse.txt");

    string fileToDelete = "data/warehouse_" + id + ".txt";
    if (filesystem::exists(fileToDelete)) {
        filesystem::remove(fileToDelete);
    }

    if (found)
        cout << "🗑️ Warehouse " << id << " deleted successfully.\n";
    else
        cout << "❌ Warehouse ID not found.\n";
}

bool Warehouse::productExists(const string& productID) {
    ifstream in("data/products.txt");
    string line;
    while (getline(in, line)) {
        if (line.find(productID) == 0) return true;
    }
    return false;
}

pair<string, float> Warehouse::fetchProductDetails(const string& productID) {
    ifstream in("data/products.txt");
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string id, name, category, brand, desc, priceStr, quantity;
        getline(ss, id, ',');
        if (id == productID) {
            getline(ss, name, ',');      // name
            for (int i = 0; i < 3; ++i) getline(ss, desc, ','); // skip category, brand, desc
            getline(ss, priceStr, ','); // price
            return {name, stof(priceStr)};
        }
    }
    return {"", 0.0};
}

void Warehouse::feedInventory() {
    string id;
    cout << "Enter Warehouse ID to feed inventory: ";
    cin >> id;

    string file = "data/warehouse_" + id + ".txt";
    if (!filesystem::exists(file)) {
        cout << "❌ Warehouse file doesn't exist.\n";
        return;
    }

    string prodID;
    int quantity;
    cout << "Enter Product ID to add/update: ";
    cin >> prodID;

    if (!productExists(prodID)) {
        cout << "❌ Product not found in central catalog.\n";
        return;
    }

    cout << "Enter Quantity: ";
    cin >> quantity;

    auto [name, price] = fetchProductDetails(prodID);

    // Check if product already exists in warehouse file
    ifstream in(file);
    ofstream temp("data/temp.txt");
    string line;
    bool updated = false;

    while (getline(in, line)) {
        stringstream ss(line);
        string pid, pname, qtyStr, priceStr;
        getline(ss, pid, ',');

        if (pid == prodID) {
            temp << prodID << "," << name << "," << quantity << "," << price << "\n";
            updated = true;
        } else {
            temp << line << "\n";
        }
    }
    in.close();

    if (!updated) {
        temp << prodID << "," << name << "," << quantity << "," << price << "\n";
    }

    temp.close();

    remove(file.c_str());
    rename("data/temp.txt", file.c_str());

    cout << "✅ Inventory updated for product " << prodID << " in warehouse " << id << ".\n";
}

void Warehouse::showAllWarehouses() {
    ifstream in("data/master_warehouse.txt");
    string line;
    cout << "\n📦 Registered Warehouses:\n";
    cout << "-------------------------------------------\n";
    while (getline(in, line)) {
        cout << line << endl;
    }
    cout << "-------------------------------------------\n";
}
