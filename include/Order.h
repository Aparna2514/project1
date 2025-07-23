#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <utility>
#include "Product.h"
#include "Inventory.h"
#include "Customer.h"

class Order {
private:
    int orderId;
    int customerId;
    std::string date;
    std::vector<std::pair<int, int>> items; // {productId, quantity}
    std::string warehouseId;
    std::string warehousePincode;
    std::string customerPincode;

public:
    Order();
    Order(int orderId, int customerId, const std::string& date,
          const std::vector<std::pair<int, int>>& items);

    // Getters
    int getOrderId() const;
    int getCustomerId() const;
    std::string getDate() const;
    std::vector<std::pair<int, int>> getItems() const;

    // Setters
    void setCustomer(const Customer& customer);

    // Computation
    double getTotalAmount(const Inventory& inventory) const;

    // File I/O
    void saveToFile(const std::string& filename, const Inventory& inventory) const;

    // Static
    static int generateOrderId();
    static Order placeOrder(const Customer& customer, Inventory& inventory);
};

#endif // ORDER_H
