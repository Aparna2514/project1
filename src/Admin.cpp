#include "Admin.h"
#include "Inventory.h"
#include "Product.h"
#include "Customer.h"
#include "Order.h"
#include "Utility.h"
#include "Warehouse.h"
#include "../analytics/Analytics.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Static Inventory object shared across Admin operations
static Inventory inventory;

bool Admin::login() {
    string username, password;
    cout << "Admin Login\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    return (username == "admin" && password == "admin123");
}

void Admin::showMenu() {
    inventory.loadFromFile();  // Load once

    int choice;
    do {
        cout << "\n Admin Dashboard:\n";
        cout << "1. View All Products\n";
        cout << "2. Add Product\n";
        cout << "3. Delete Product\n";
        cout << "4. Restock Product\n";
        cout << "5. View Low Stock Alerts\n";
        cout << "6. View All Customers\n";
        cout << "7. View All Orders\n";
        cout << "8. View Analytics & Reports\n";
        cout << "9. Manage Warehouses\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewAllProducts(); break;
            case 2: addProduct(); break;
            case 3: deleteProduct(); break;
            case 4: restockProduct(); break;
            case 5: viewLowStockAlerts(); break;
            case 6: viewCustomers(); break;
            case 7: viewOrders(); break;
            case 8: viewAnalyticsReports(); break;
            case 9: manageWarehouses(); break;
            case 0: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice.\n"; break;
        }
    } while (choice != 0);
}

void Admin::viewAllProducts() {
    cout << "\n Product List:\n";
    inventory.showAllProducts();  // ✅ fixed
}

void Admin::addProduct() {
    int id = Utility::generateID();
    string name, category, brand, description;
    double price, discount, gst;
    int quantity, threshold;

    cin.ignore();
    cout << "Enter product name: "; getline(cin, name);
    cout << "Enter category: "; getline(cin, category);
    cout << "Enter brand: "; getline(cin, brand);
    cout << "Enter description: "; getline(cin, description);
    cout << "Enter price: "; cin >> price;
    cout << "Enter quantity: "; cin >> quantity;
    cout << "Enter discount (%): "; cin >> discount;
    cout << "Enter GST (%): "; cin >> gst;
    cout << "Enter reorder threshold: "; cin >> threshold;

    Product newProduct(id, name, category, brand, description,
                       price, quantity, discount, gst, threshold);
    inventory.addProduct(newProduct);
    inventory.saveToFile();

    cout << " Product added successfully.\n";
}

void Admin::deleteProduct() {
    int id;
    cout << "Enter Product ID to delete: ";
    cin >> id;

    if (inventory.removeProduct(id)) {
        inventory.saveToFile();
        cout << " Product deleted.\n";
    } else {
        cout << " Product not found.\n";
    }
}

void Admin::restockProduct() {
    int id, quantity;
    cout << "Enter Product ID to restock: ";
    cin >> id;
    cout << "Enter quantity to add: ";
    cin >> quantity;

    if (inventory.updateStock(id, quantity)) {
        inventory.saveToFile();
        cout << " Stock updated.\n";
    } else {
        cout << "Product not found.\n";
    }
}

void Admin::viewLowStockAlerts() {
    int threshold = 5;  // Default value, or take user input
    inventory.showLowStock(threshold);  // ✅ fixed
}

void Admin::viewCustomers() {
    ifstream file("data/customers.txt");
    string line;
    cout << "\n👥 Registered Customers:\n";
    while (getline(file, line)) {
        replace(line.begin(), line.end(), '|', ' ');
        cout << line << "\n";
    }
}

void Admin::viewOrders() {
    ifstream file("data/orders.txt");
    string line;
    cout << "\n All Orders:\n";
    while (getline(file, line)) {
        replace(line.begin(), line.end(), '|', ' ');
        cout << line << "\n";
    }
}

// 8th feature
void Admin::viewAnalyticsReports() {
    int choice;
    cout << "\n Analytics & Reports:\n";
    cout << "1. Sales Report (Daily)\n";
    cout << "2. Sales Report (Weekly)\n";
    cout << "3. Sales Report (Monthly)\n";
    cout << "4. Most/Least Ordered Products\n";
    cout << "5. Customer Order Trends\n";
    cout << "0. Back to Admin Menu\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1: generateSalesReport("daily"); break;
        case 2: generateSalesReport("weekly"); break;
        case 3: generateSalesReport("monthly"); break;
        case 4: showMostAndLeastOrderedProducts(); break;
        case 5: showCustomerOrderTrends(); break;
        case 0: return;
        default: cout << " Invalid choice.\n";
    }
}

void Admin::manageWarehouses() {
    int choice;
    do {
        cout << "\n Warehouse Management:\n";
        cout << "1. Register New Warehouse\n";
        cout << "2. Delete Warehouse\n";
        cout << "3. Feed/Update Inventory in Warehouse\n";
        cout << "4. View All Warehouses\n";
        cout << "0. Back to Admin Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerWarehouse(); break;
            case 2: deleteWarehouse(); break;
            case 3: feedWarehouseInventory(); break;
            case 4: viewAllWarehouses(); break;
            case 0: return;
            default: cout << " Invalid choice.\n";
        }
    } while (choice != 0);
}

void Admin::registerWarehouse() {
    Warehouse w;
    w.registerWarehouse();
}

void Admin::deleteWarehouse() {
    string id;
    cout << "Enter Warehouse ID to delete: ";
    cin >> id;
    Warehouse w;
    w.deleteWarehouse(id);
}

void Admin::feedWarehouseInventory() {
    Warehouse w;
    w.feedInventory();
}

void Admin::viewAllWarehouses() {
    Warehouse::showAllWarehouses();
}
