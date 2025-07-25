#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <ctime>
#include <algorithm>
#include "Analytics.h"

std::tm parseDate(const std::string& dateStr) {
    std::tm t = {};
    sscanf(dateStr.c_str(), "%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday);
    t.tm_year -= 1900;
    t.tm_mon -= 1;
    return t;
}

bool dateMatches(std::tm orderDate, const std::string& timeframe) {
    std::time_t now = std::time(nullptr);
    std::tm* current = std::localtime(&now);

    if (timeframe == "daily") {
        return (orderDate.tm_year == current->tm_year &&
                orderDate.tm_mon == current->tm_mon &&
                orderDate.tm_mday == current->tm_mday);
    }
    else if (timeframe == "weekly") {
        std::time_t t1 = std::mktime(current);
        std::time_t t2 = std::mktime(&orderDate);
        double days = std::difftime(t1, t2) / (60 * 60 * 24);
        return days <= 7;
    }
    else if (timeframe == "monthly") {
        return (orderDate.tm_year == current->tm_year &&
                orderDate.tm_mon == current->tm_mon);
    }

    return false;
}

void generateSalesReport(const std::string& timeframe) {
    std::ifstream file("data/orders.txt");
    if (!file) {
        std::cerr << "Error opening orders.txt\n";
        return;
    }

    std::string line;
    double totalRevenue = 0;
    int totalOrders = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string orderID, customerID, dateStr, productData;

        std::getline(ss, orderID, ',');
        std::getline(ss, customerID, ',');
        std::getline(ss, dateStr, ',');
        std::getline(ss, productData);

        std::tm orderDate = parseDate(dateStr);
        if (!dateMatches(orderDate, timeframe)) continue;

        totalOrders++;
        std::stringstream prodStream(productData);
        std::string prodItem;

        while (std::getline(prodStream, prodItem, ';')) {
            if (prodItem.empty()) continue;

            std::stringstream itemStream(prodItem);
            std::string prodID, qtyStr, priceStr;

            std::getline(itemStream, prodID, ':');
            std::getline(itemStream, qtyStr, ':');
            std::getline(itemStream, priceStr, ':');

            int qty = std::stoi(qtyStr);
            double price = std::stod(priceStr);
            totalRevenue += price;
        }
    }

    std::cout << " Sales Report (" << timeframe << "):\n";
    if (totalOrders == 0) {
        std::cout << "No orders found in the selected timeframe (" << timeframe << ").\n";
        std::cout << " Tip: Place a few test orders to see analytics in action.\n\n";
        return;
    }

    std::cout << "Total Orders: " << totalOrders << "\n";
    std::cout << "Total Revenue: ₹" << totalRevenue << "\n\n";
}


void showMostAndLeastOrderedProducts() {
    std::ifstream file("data/orders.txt");
    if (!file) {
        std::cerr << "Error opening orders.txt\n";
        return;
    }

    std::map<std::string, int> productCounts;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string skip1, skip2, skip3, productData;
        std::getline(ss, skip1, ','); // OrderID
        std::getline(ss, skip2, ','); // CustomerID
        std::getline(ss, skip3, ','); // Date
        std::getline(ss, productData);

        std::stringstream prodStream(productData);
        std::string prodItem;
        while (std::getline(prodStream, prodItem, ';')) {
            if (prodItem.empty()) continue;
            std::stringstream itemStream(prodItem);
            std::string prodID, qtyStr, priceStr;

            std::getline(itemStream, prodID, ':');
            std::getline(itemStream, qtyStr, ':');
            std::getline(itemStream, priceStr, ':');

            int qty = std::stoi(qtyStr);
            productCounts[prodID] += qty;
        }
    }

    if (productCounts.empty()) {
        std::cout << "No product data available.\n";
        return;
    }

    auto most = std::max_element(productCounts.begin(), productCounts.end(),
        [](auto& a, auto& b) { return a.second < b.second; });

    auto least = std::min_element(productCounts.begin(), productCounts.end(),
        [](auto& a, auto& b) { return a.second < b.second; });

    std::cout << "Most Ordered Product: " << most->first << " (" << most->second << " units)\n";
    std::cout << " Least Ordered Product: " << least->first << " (" << least->second << " units)\n\n";
}

void showCustomerOrderTrends() {
    std::ifstream file("data/orders.txt");
    if (!file) {
        std::cerr << "Error opening orders.txt\n";
        return;
    }

    std::map<std::string, int> customerOrders;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string orderID, customerID, dateStr, productData;

        std::getline(ss, orderID, ',');
        std::getline(ss, customerID, ',');
        std::getline(ss, dateStr, ',');
        std::getline(ss, productData);

        std::stringstream prodStream(productData);
        std::string prodItem;

        while (std::getline(prodStream, prodItem, ';')) {
            if (prodItem.empty()) continue;
            std::stringstream itemStream(prodItem);
            std::string prodID, qtyStr, priceStr;

            std::getline(itemStream, prodID, ':');
            std::getline(itemStream, qtyStr, ':');
            std::getline(itemStream, priceStr, ':');

            int qty = std::stoi(qtyStr);
            customerOrders[customerID] += qty;
        }
    }

    std::cout << " Customer Order Trends:\n";
    for (const auto& [cid, totalQty] : customerOrders) {
        std::cout << "Customer ID " << cid << ": " << totalQty << " units ordered\n";
    }
    std::cout << '\n';
}
