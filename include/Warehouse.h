#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <string>
#include <vector>

class Warehouse {
private:
    std::string warehouseID;
    std::string exactAddress;
    std::string pincode;
    std::string status;

    std::string getWarehouseFileName() const;
    bool productExists(const std::string& productID);
    std::pair<std::string, float> fetchProductDetails(const std::string& productID); // name, price

public:
    Warehouse();  // default constructor
    Warehouse(std::string id, std::string address, std::string pin, std::string stat); // parameterized

    void registerWarehouse();                      // Register new warehouse
    void deleteWarehouse(const std::string& id);   // Delete warehouse and its file
    void feedInventory();                          // Add/modify inventory to warehouse file

    static void showAllWarehouses();               // Display all warehouses

    // Getters used by ProximityManager
    std::string getID() const;
    std::string getPincode() const;
    std::string getAddress() const;
    std::string getStatus() const;
};

#endif
