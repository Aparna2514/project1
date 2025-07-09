#include "Order.h"
#include "Utility.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> // for iomanip
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

// Save order to file (explicit path provided by caller)
void Order::saveToFile(const std::string& filename, const Inventory& inventory) const {
    std::ofstream out(filename, std::ios::app);
    if (!out) {
        std::cerr << "Failed to open order file.\n";
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

        out << pid << ":" << qty << ":" << std::fixed << std::setprecision(2) << finalPrice;

        if (i != items.size() - 1) out << ";";
    }

    out << "\n";
    out.close();
}

// Generate unique order ID
int Order::generateOrderId() {
    return Utility::generateID();
}

// Place order from user input and save to file
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
    newOrder.saveToFile("data/orders.txt", inventory);  // ✅ Explicit file path
    std::cout << "Order placed successfully. Order ID: " << oid << "\n";
    return newOrder;
}

void Order::setCustomer(const Customer& customer) {
    this->customerId = customer.getCustomerId();
}
