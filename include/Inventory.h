#ifndef INVENTORY_H
#define INVENTORY_H

#include "Product.h"
#include <vector>
#include <string>

class Inventory {
private:
    std::vector<Product> products;
    std::string filePath = "data/products.txt";

public:
    Inventory();

    void loadFromFile();
    void saveToFile() const;

    void addProduct(const Product& p);
    bool removeProduct(int productId);
    Product* getProductById(int productId) const;
    std::vector<Product> searchByName(const std::string& name) const;

    bool updateStock(int productId, int quantityChange);
    void showAllProducts() const;
    void showLowStock(int threshold = 5) const;
    void displayProductForCustomer() const;
    //std::vector<Product> getLowStockProducts(int threshold) const;

    // added as helper function for order.txt
    //static Product* getProductStaticById(int productId);

};


#endif