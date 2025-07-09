#include "Invoice.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <sstream>

void Invoice::generateInvoice(const Order& order, const Inventory& inventory, const Customer& customer) {
    std::ostringstream filename;
    filename << "invoices/invoice_" << order.getOrderId() << ".txt";
    std::ofstream file(filename.str());

    if (!file) {
        std::cerr << "Failed to create invoice file.\n";
        return;
    }

    // Get current date-time
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::ostringstream dateTime;
    dateTime << 1900 + ltm->tm_year << "-"
             << std::setw(2) << std::setfill('0') << 1 + ltm->tm_mon << "-"
             << std::setw(2) << std::setfill('0') << ltm->tm_mday << " "
             << std::setw(2) << std::setfill('0') << ltm->tm_hour << ":"
             << std::setw(2) << std::setfill('0') << ltm->tm_min;

    // Header
    file << "=====================================================\n";
    file << "                 INVOICE - ORDER #" << order.getOrderId() << "\n";
    file << "=====================================================\n";
    file << "Customer ID: " << customer.getCustomerId() << "\n";
    file << "Date: " << dateTime.str() << "\n";
    file << "-----------------------------------------------------\n";
    file << std::left << std::setw(15) << "Product"
         << std::setw(8) << "Qty"
         << std::setw(10) << "Price"
         << std::setw(10) << "Discount"
         << std::setw(6) << "GST"
         << std::setw(10) << "Total\n";
    file << "-----------------------------------------------------\n";

    double grandTotal = 0.0;

    for (const auto& item : order.getItems()) {
        int pid = item.first;
        int qty = item.second;
        Product* product = inventory.getProductById(pid);

        if (!product) continue;

        double basePrice = product->getPrice() * qty;
        double discountAmt = basePrice * product->getDiscountPercent() / 100;
        double priceAfterDiscount = basePrice - discountAmt;
        double gstAmt = priceAfterDiscount * product->getGstPercent() / 100;
        double total = priceAfterDiscount + gstAmt;

        grandTotal += total;

        file << std::left << std::setw(15) << product->getName()
             << std::setw(8) << qty
             << std::setw(10) << product->getPrice()
             << std::setw(10) << product->getDiscountPercent() << "%"
             << std::setw(6) << product->getGstPercent() << "%"
             << std::setw(10) << std::fixed << std::setprecision(2) << total << "\n";
    }

    file << "-----------------------------------------------------\n";
    file << std::right << std::setw(50) << "Grand Total: ₹" << grandTotal << "\n";
    file << "=====================================================\n";
    file.close();

    std::cout << "✅ Invoice generated: " << filename.str() << "\n";
    //modified for default open
    std::string openCommand = "start " + filename.str();
    system(openCommand.c_str());

}

