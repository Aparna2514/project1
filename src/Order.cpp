#include "Order.h"
#include "Utility.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>

// Default constructor
Order::Order() : orderId(0), customerId(0), date("") {}

// Parameterized constructor
Order::Order(int orderId, int customerId, const std::string& date,
             const std::vector<std::pair<int, int>>& items)
    : orderId(orderId), customerId(customerId), date(date), items(items) {}

int Order::getOrderId() const { return orderId; }
int Order::getCustomerId() const { return customerId; }
std::string Order::getDate() const { return date; }
std::vector<std::pair<int, int>> Order::getItems() const { return items; }

// Calculate total order cost
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

// Save order to file
void Order::saveToFile(const std::string& filename) const {
    std::ofstream out(filename, std::ios::app);
    if (!out) {
        std::cerr << "Failed to open order file.\n";
        return;
    }
    out << orderId << "," << customerId << "," << date << ",";
    for (size_t i = 0; i < items.size(); ++i) {
        out << items[i].first << ":" << items[i].second;
        if (i != items.size() - 1) out << ";";
    }
    out << "\n";
    out.close();
}

// Generate unique order ID (dummy for now, utility logic later)
int Order::generateOrderId() {
    return Utility::generateID();
}

// Place order (basic input simulation here)
Order Order::placeOrder(const Customer& customer, Inventory& inventory) {
    std::vector<std::pair<int, int>> orderItems;
    char more = 'y';

    while (more == 'y' || more == 'Y') {
        int pid, qty;
        std::cout << "Enter Product ID to order: ";
        std::cin >> pid;
        std::cout << "Enter quantity: ";
        std::cin >> qty;

        Product* product = inventory.getProductById(pid);
        if (!product) {
            std::cout << "Invalid Product ID.\n";
        } else if (qty > product->getQuantity()) {
            std::cout << "Not enough stock.\n";
        } else {
            orderItems.push_back({pid, qty});
            inventory.updateStock(pid, -qty);
            std::cout << "Product added to order.\n";
        }

        std::cout << "Add another product? (y/n): ";
        std::cin >> more;
    }

    // Get current date
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::ostringstream oss;
    oss << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday;
    std::string date = oss.str();

    int oid = generateOrderId();

    Order newOrder(oid, customer.getCustomerId(), date, orderItems);
    newOrder.saveToFile(); // saves to orders.txt
    std::cout << "Order placed successfully. Order ID: " << oid << "\n";
    return newOrder;
}

void Order::setCustomer(const Customer& customer) {
    this->customerId = customer.getCustomerId();
}
