#include "../include/proximity.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <iostream>

using namespace std;

void ProximityManager::loadWarehouses(const string& filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string id, addr, pin, status;

        getline(ss, id, ',');
        getline(ss, addr, ',');
        getline(ss, pin, ',');
        getline(ss, status, ',');

        if (status == "Active") {
            warehouses.emplace_back(id, addr, pin, status);  // uses real Warehouse constructor
        }
    }
}

int ProximityManager::hammingDistance(const string& a, const string& b) const {
    if (a.length() != b.length()) return 999;
    int dist = 0;
    for (size_t i = 0; i < a.length(); ++i) {
        if (a[i] != b[i]) dist++;
    }
    return dist;
}

bool ProximityManager::checkAvailability(const string& warehouseID, const map<string, int>& productQtyMap) {
    ifstream file("data/warehouse_" + warehouseID + ".txt");
    map<string, int> inventory;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string productID, name, qtyStr, priceStr;
        getline(ss, productID, ',');
        getline(ss, name, ',');
        getline(ss, qtyStr, ',');
        getline(ss, priceStr, ',');
        inventory[productID] = stoi(qtyStr);
    }

    for (const auto& item : productQtyMap) {
        if (inventory[item.first] < item.second)
            return false;
    }
    return true;
}

string ProximityManager::findBestWarehouse(const string& customerPincode, const map<string, int>& productQtyMap) {
    // Step 1: Exact Match
    for (const auto& wh : warehouses) {
        if (wh.getPincode() == customerPincode) {
            if (checkAvailability(wh.getID(), productQtyMap))
                return wh.getID();
        }
    }

    // Step 2: Prefix Match (first 4 digits)
    for (const auto& wh : warehouses) {
        if (wh.getPincode().substr(0, 4) == customerPincode.substr(0, 4)) {
            if (checkAvailability(wh.getID(), productQtyMap))
                return wh.getID();
        }
    }

    // Step 3: Hamming Distance match
    vector<pair<int, Warehouse>> distances;
    for (const auto& wh : warehouses) {
        int dist = hammingDistance(customerPincode, wh.getPincode());
        distances.emplace_back(dist, wh);
    }

    sort(distances.begin(), distances.end(), [](auto& a, auto& b) {
        return a.first < b.first;
    });

    for (const auto& [dist, wh] : distances) {
        if (checkAvailability(wh.getID(), productQtyMap))
            return wh.getID();
    }

    return "NOT_FOUND";
}
