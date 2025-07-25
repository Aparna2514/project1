#ifndef PROXIMITY_H
#define PROXIMITY_H

#include <string>
#include <vector>
#include <map>
#include "Warehouse.h"  // include the real Warehouse class

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
