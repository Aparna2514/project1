#include "Admin.h"
#include "Inventory.h"
#include "Product.h"
#include "Customer.h"
#include "Order.h"
#include "Invoice.h"
#include "Utility.h"

#include <iostream>
using namespace std;

Inventory inventory;  // Global shared inventory

void customerMenu() {
    int choice;
    Customer currentCustomer;
    bool isRegistered = false;

    cout << "\n1. Register as New Customer\n2. Continue as Guest\nEnter choice: ";
    cin >> choice;

    if (choice == 1) {
        currentCustomer = currentCustomer.registerCustomer();
        isRegistered = true;
    } else {
       currentCustomer = Customer(0, "Guest", "guest@example.com", "N/A", "0000000000", "000000");
    }

    do {
        cout << "\n--- Customer Menu ---\n";
        cout << "1. View Available Products\n";
        cout << "2. Place an Order\n";
        cout << "3. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                
                inventory.displayProductForCustomer();
                break;
            case 2: {
                Order order = order.placeOrder(currentCustomer, inventory);
                Invoice invoice;
                invoice.generateInvoice(order, inventory, currentCustomer);
                cout << " Order placed successfully. Invoice generated!\n";
                break;
            }
            case 3:
                cout << "Returning to Main Menu...\n";
                break;
            default:
                cout << " Invalid choice.\n";
                break;
        }
    } while (choice != 3);
}

// modified full dashboard for admin
void adminAccess() {
    Admin admin;
    if (admin.login()) {
        admin.showMenu();  // Full admin dashboard
    } else {
        cout << " Login failed. Returning to main menu.\n";
    }
}

int main() {
    inventory.loadFromFile();  // Load all product data at start

    int choice;
    do {
        cout << "\n===  Supply Chain Management System ===\n";
        cout << "1. Admin Login\n";
        cout << "2. Customer Access\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: adminAccess(); break;
            //modified for solving inconsistecy in customer data
            case 2: inventory.loadFromFile(); customerMenu(); break;
            case 3: cout << "Exiting system. Goodbye!\n"; break;
            default: cout << " Invalid option.\n"; break;
        }

    } while (choice != 3);

    inventory.saveToFile();  // Save changes before exiting
    return 0;
}