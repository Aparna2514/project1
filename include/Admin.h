#ifndef ADMIN_H
#define ADMIN_H

class Admin {
public:
    static bool login();
    static void showMenu();

private:
    static void viewAllProducts();
    static void addProduct();
    static void deleteProduct();
    static void restockProduct();
    static void viewLowStockAlerts();
    static void viewCustomers();
    static void viewOrders();
};

#endif // ADMIN_H