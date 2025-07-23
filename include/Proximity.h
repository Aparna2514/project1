#ifndef PROXIMITY_H
#define PROXIMITY_H

#include <string>
#include <vector>
#include <map>

class Warehouse {
private:
    std::string warehouseID;
    std::string address;
    std::string pincode;

public:
    Warehouse(const std::string& id, const std::string& addr, const std::string& pin);
    std::string getID() const;
    std::string getPincode() const;
    std::string getAddress() const;
};

class ProximityManager {
private:
    std::vector<Warehouse> warehouses;

    int hammingDistance(const std::string& a, const std::string& b) const;
    bool checkAvailability(const std::string& warehouseID, const std::map<std::string, int>& productQtyMap);

public:
    void loadWarehouses(const std::string& filename);
    std::string findBestWarehouse(const std::string& customerPincode, const std::map<std::string, int>& productQtyMap);
};

#endif
