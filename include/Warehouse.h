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
    Warehouse(std::string id, std::string address, std::string pin, std::string stat);

    void registerWarehouse();                // 1. Register new warehouse
    void deleteWarehouse(const std::string& id); // 3. Delete warehouse + file
    void feedInventory();                    // 5. Add/modify inventory to warehouse file

    static void showAllWarehouses();         // utility to display all warehouses
};

#endif
