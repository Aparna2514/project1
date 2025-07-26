#include "Order.h"
#include "Utility.h"
#include "Proximity.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <filesystem>
using namespace std;

Order::Order() : orderId(0), customerId(0), date("") {}

Order::Order(int orderId, int customerId, const string& date,
             const vector<pair<int, int>>& items)
    : orderId(orderId), customerId(customerId), date(date), items(items) {}

int Order::getOrderId() const { return orderId; }
int Order::getCustomerId() const { return customerId; }
string Order::getDate() const { return date; }
vector<pair<int, int>> Order::getItems() const { return items; }

void Order::setCustomer(const Customer& customer) {
    this->customerId = customer.getCustomerId();
}

double Order::getTotalAmount(const Inventory& inventory) const {
    double total = 0.0;
    for (const auto& item : items) {
        Product* product = inventory.getProductById(item.first);
        if (product) {
            double base = product->getPrice() * item.second;
            double afterDiscount = base - (base * product->getDiscountPercent() / 100);
            double withGST = afterDiscount + (afterDiscount * product->getGstPercent() / 100);
            total += withGST;
        }
    }
    return total;
}

int Order::generateOrderId() {
    return Utility::generateID();
}

void Order::saveToFile(const string& filename, const Inventory& inventory) const {
    ofstream out(filename, ios::app);
    if (!out) {
        cerr << "Failed to open order file.\n";
        return;
    }

    out << orderId << "," << customerId << "," << date << ",";

    for (size_t i = 0; i < items.size(); ++i) {
        int pid = items[i].first;
        int qty = items[i].second;

        Product* product = inventory.getProductById(pid);
        if (!product) continue;

        double base = product->getPrice() * qty;
        double afterDiscount = base - (base * product->getDiscountPercent() / 100);
        double finalPrice = afterDiscount + (afterDiscount * product->getGstPercent() / 100);

        out << pid << ":" << qty << ":" << fixed << setprecision(2) << finalPrice;
        if (i != items.size() - 1) out << ";";
    }

    out << "," << warehouseId << "," << warehousePincode << "," << customerPincode << "\n";
    out.close();
}

Order Order::placeOrder(const Customer& customer, Inventory& inventory) {
    vector<pair<int, int>> orderItems;
    char more = 'y';

    while (more == 'y' || more == 'Y') {
        int pid, qty;
        cout << "Enter Product ID to order: ";
        cin >> pid;
        cout << "Enter quantity: ";
        cin >> qty;

        Product* product = inventory.getProductById(pid);
        if (!product) {
            cout << "Invalid Product ID.\n";
        } else {
            orderItems.push_back({pid, qty});
            cout << "Product added to order.\n";
        }

        cout << "Add another product? (y/n): ";
        cin >> more;
    }

    // Prepare productQtyMap for availability check
    map<string, int> productQtyMap;
    for (auto& item : orderItems) {
        productQtyMap[to_string(item.first)] = item.second;
    }

    ProximityManager pm;
    pm.loadWarehouses("data/master_warehouse.txt");

    string custPin = customer.getPincode();
    string bestWarehouseId = pm.findBestWarehouse(custPin, productQtyMap);

    if (bestWarehouseId == "NOT_FOUND") {
        cout << " No warehouse can fulfill the entire order.\n";
        return Order();
    }

    // Deduct stock
    string warehouseFile = "data/warehouse_" + bestWarehouseId + ".txt";
    ifstream wf(warehouseFile);
    if (!wf) {
        cout << " Warehouse data file not found.\n";
        return Order();
    }

    map<string, pair<int, float>> stock;
    string line;
    while (getline(wf, line)) {
        stringstream ss(line);
        string pid, pname, qtyStr, priceStr;
        getline(ss, pid, ',');
        getline(ss, pname, ',');
        getline(ss, qtyStr, ',');
        getline(ss, priceStr, ',');

        stock[pid] = {stoi(qtyStr), stof(priceStr)};
    }
    wf.close();

    for (auto& item : orderItems) {
        string idStr = to_string(item.first);
        stock[idStr].first -= item.second;
    }

    // Rewrite updated stock
    ofstream out(warehouseFile);
    for (auto& [pid, pairVal] : stock) {
        out << pid << "," << "Unknown" << "," << pairVal.first << "," << fixed << setprecision(2) << pairVal.second << "\n";
    }
    out.close();

    // Get warehouse pincode
    string selectedWarehousePin;
    ifstream master("data/master_warehouse.txt");
    string row;
    while (getline(master, row)) {
        stringstream ms(row);
        string id, addr, pin;
        getline(ms, id, ',');
        getline(ms, addr, ',');
        getline(ms, pin, ',');
        if (id == bestWarehouseId) {
            selectedWarehousePin = pin;
            break;
        }
    }
    master.close();

    // Prepare and save order
    int oid = generateOrderId();
    Order newOrder(oid, customer.getCustomerId(), Utility::getCurrentTimestamp(), orderItems);
    newOrder.warehouseId = bestWarehouseId;
    newOrder.warehousePincode = selectedWarehousePin;
    newOrder.customerPincode = custPin;

    newOrder.saveToFile("data/orders.txt", inventory);

    cout << " Order placed successfully from warehouse: " << bestWarehouseId << "\n";
    cout << "Order ID: " << oid << "\n";

    return newOrder;
}
