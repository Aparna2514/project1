#include "../include/proximity.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

Warehouse::Warehouse(const std::string& id, const std::string& addr, const std::string& pin)
    : warehouseID(id), address(addr), pincode(pin) {}

std::string Warehouse::getID() const { return warehouseID; }
std::string Warehouse::getPincode() const { return pincode; }
std::string Warehouse::getAddress() const { return address; }

void ProximityManager::loadWarehouses(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, addr, pin;

        getline(ss, id, ',');
        getline(ss, addr, ',');
        getline(ss, pin, ',');

        warehouses.emplace_back(id, addr, pin);
    }
}

int ProximityManager::hammingDistance(const std::string& a, const std::string& b) const {
    if (a.length() != b.length()) return 999;
    int dist = 0;
    for (size_t i = 0; i < a.length(); ++i) {
        if (a[i] != b[i]) dist++;
    }
    return dist;
}

bool ProximityManager::checkAvailability(const std::string& warehouseID, const std::map<std::string, int>& productQtyMap) {
    std::ifstream file("data/warehouse_" + warehouseID + ".txt");
    std::map<std::string, int> inventory;
    std::string line;

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string productID, qtyStr;
        getline(ss, productID, ',');
        getline(ss, qtyStr, ',');
        inventory[productID] = std::stoi(qtyStr);
    }

    for (const auto& item : productQtyMap) {
        if (inventory[item.first] < item.second)
            return false;
    }
    return true;
}

std::string ProximityManager::findBestWarehouse(const std::string& customerPincode, const std::map<std::string, int>& productQtyMap) {
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
    std::vector<std::pair<int, Warehouse>> distances;
    for (const auto& wh : warehouses) {
        int dist = hammingDistance(customerPincode, wh.getPincode());
        distances.emplace_back(dist, wh);
    }

    std::sort(distances.begin(), distances.end(), [](auto& a, auto& b) {
        return a.first < b.first;
    });

    for (const auto& [dist, wh] : distances) {
        if (checkAvailability(wh.getID(), productQtyMap))
            return wh.getID();
    }

    return "NOT_FOUND";
}
