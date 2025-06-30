#include "Inventory.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

Inventory::Inventory() {
    loadFromFile();
}

void Inventory::loadFromFile() {
    products.clear();
    ifstream file(filePath);
    if (!file) {
        cerr << "Error: Could not open " << filePath << " for reading.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        int id, quantity, reorderThreshold;
        double price, discount, gst;
        string name, category, brand, description;

        getline(ss, line, ','); id = stoi(line);
        getline(ss, name, ',');
        getline(ss, category, ',');
        getline(ss, brand, ',');
        getline(ss, description, ',');
        getline(ss, line, ','); price = stod(line);
        getline(ss, line, ','); quantity = stoi(line);
        getline(ss, line, ','); discount = stod(line);
        getline(ss, line, ','); gst = stod(line);
        getline(ss, line, ','); reorderThreshold = stoi(line);

        Product p(id, name, category, brand, description, price, quantity, discount, gst, reorderThreshold);
        products.push_back(p);
    }

    file.close();
}

void Inventory::saveToFile() const {
    ofstream file(filePath);
    if (!file) {
        cerr << "Error: Could not open " << filePath << " for writing.\n";
        return;
    }

    for (const auto& p : products) {
        file << p.getProductId() << ","
             << p.getName() << ","
             << p.getCategory() << ","
             << p.getBrand() << ","
             << p.getDescription() << ","
             << p.getPrice() << ","
             << p.getQuantity() << ","
             << p.getDiscountPercent() << ","
             << p.getGstPercent() << ","
             << p.getReorderThreshold() << "\n";
    }

    file.close();
}

void Inventory::addProduct(const Product& p) {
    products.push_back(p);
    saveToFile();
}

bool Inventory::removeProduct(int productId) {
    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->getProductId() == productId) {
            products.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

 Product* Inventory::getProductById(int productId) const {
    for (auto& p : const_cast<vector<Product>&>(products)) {
        if (p.getProductId() == productId) {
            return &p;
        }
    }
    return nullptr;
}

vector<Product> Inventory::searchByName(const string& name) const {
    vector<Product> results;
    for (const auto& p : products) {
        if (p.getName().find(name) != string::npos) {
            results.push_back(p);
        }
    }
    return results;
}

bool Inventory::updateStock(int productId, int quantityChange) {
     Product* p = getProductById(productId);
    if (p) {
        int currentQty = p->getQuantity();
        if (currentQty + quantityChange < 0) return false;
        p->setQuantity(currentQty + quantityChange);
        saveToFile();
        return true;
    }
    return false;
}

void Inventory::showAllProducts() const {
    cout << left << setw(6) << "ID"
         << setw(15) << "Name"
         << setw(15) << "Category"
         << setw(12) << "Brand"
         << setw(20) << "Description"
         << setw(10) << "Price"
         << setw(8) << "Qty"
         << setw(10) << "Discount(%)"
         << setw(8) << "GST(%)"
         << setw(10) << "Reorder" << "\n";

    cout << string(124, '-') << "\n";

    for (const auto& p : products) {
        cout << left << setw(6) << p.getProductId()
             << setw(15) << p.getName()
             << setw(15) << p.getCategory()
             << setw(12) << p.getBrand()
             << setw(20) << p.getDescription()
             << setw(10) << p.getPrice()
             << setw(8) << p.getQuantity()
             << setw(10) << p.getDiscountPercent()
             << setw(8) << p.getGstPercent()
             << setw(10) << p.getReorderThreshold()
             << "\n";
    }
}

void Inventory::showLowStock(int threshold) const {
    cout << "\n⚠️  Low Stock Products (Below " << threshold << "):\n";
    for (const auto& p : products) {
        if (p.getQuantity() < threshold) {
            cout << " - " << p.getName() << " (ID: " << p.getProductId()
                 << ", Qty: " << p.getQuantity() << ")\n";
        }
    }
}

void Inventory::displayProductForCustomer() const {
    cout << "\n🛍️  Available Products for Customers:\n";
    cout << left << setw(6) << "ID" << setw(20) << "Name"
         << setw(15) << "Brand" << setw(10) << "Price" << "\n";
    cout << "-----------------------------------------------------------\n";
    for (const auto& p : products) {
        cout << left << setw(6) << p.getProductId()
             << setw(20) << p.getName()
             << setw(15) << p.getBrand()
             << setw(10) << p.getPrice() << "\n";
    }
}

// function for auto alert 

void Inventory::showReorderAlerts() const {
    bool alertShown = false;

    for (const auto& p : products) {
        if (p.getQuantity() < p.getReorderThreshold()) {
            if (!alertShown) {
                cout << "\n🔔 LOW STOCK ALERTS:\n";
                alertShown = true;
            }
            cout << "   → " << p.getName()
                 << " (ID: " << p.getProductId() << ") "
                 << "is below threshold. Current: "
                 << p.getQuantity() << ", Threshold: "
                 << p.getReorderThreshold() << "\n";
        }
    }

    if (!alertShown) {
        cout << "\n✅ All products are above their reorder thresholds.\n";
    }
}


