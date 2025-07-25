#include "Product.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

// Constructor
Product::Product(int id, const string& name, const string& category,
                 const string& brand, const string& description,
                 double price, double discount, double gst)
    : productId(id), name(name), category(category), brand(brand), description(description),
      price(price), discountPercent(discount), gstPercent(gst) {}

// Getters
int Product::getProductId() const { return productId; }
string Product::getName() const { return name; }
string Product::getCategory() const { return category; }
string Product::getBrand() const { return brand; }
string Product::getDescription() const { return description; }
double Product::getPrice() const { return price; }
//int Product::getQuantity() const { return quantity; }
double Product::getDiscountPercent() const { return discountPercent; }
double Product::getGstPercent() const { return gstPercent; }
//int Product::getReorderThreshold() const { return reorderThreshold; }

// Setters
void Product::setName(const string& n) { name = n; }
void Product::setCategory(const string& c) { category = c; }
void Product::setBrand(const string& b) { brand = b; }
void Product::setDescription(const string& d) { description = d; }
void Product::setPrice(double p) { if (p >= 0) price = p; }
//void Product::setQuantity(int q) { if (q >= 0) quantity = q; }
void Product::setDiscountPercent(double d) { if (d >= 0) discountPercent = d; }
void Product::setGstPercent(double g) { if (g >= 0) gstPercent = g; }
// void Product::setReorderThreshold(int r) { if (r >= 0) reorderThreshold = r; }

// Display for admin
void Product::displayProduct() const {
    cout << "Product ID: " << productId << "\n"
         << "Name: " << name << "\n"
         << "Category: " << category << "\n"
         << "Brand: " << brand << "\n"
         << "Description: " << description << "\n"
         << fixed << setprecision(2)
         << "Price: ₹" << price << "\n"
         //<< "Quantity: " << quantity << "\n"
         << "Discount: " << discountPercent << "%\n"
         << "GST: " << gstPercent << "%\n";
         //<< "Reorder Threshold: " << reorderThreshold << "\n\n";
}

// Display for customers
void Product::displayForCustomer() const {
    cout << "Product: " << name << " (" << brand << ")\n"
         << "Price: ₹" << fixed << setprecision(2) << getFinalPriceWithGST() << " (after tax and discount)\n"
         << "Description: " << description << "\n";
}

/* Quantity update
void Product::updateQuantity(int change) {
    quantity += change;
    if (quantity < 0) quantity = 0;
}*/

/* Low stock check
bool Product::isLowStock() const {
    return quantity < reorderThreshold;
}*/

// Discounted price
double Product::getDiscountedPrice() const {
    return price * (1.0 - discountPercent / 100.0);
}

// Final price after GST
double Product::getFinalPriceWithGST() const {
    double discounted = getDiscountedPrice();
    return discounted * (1.0 + gstPercent / 100.0);
}

// Convert product to file string
string Product::toFileString() const {
    ostringstream oss;
    oss << productId << "," << name << "," << category << "," << brand << ","
        << description << "," << price << "," 
        << discountPercent << "," << gstPercent ;
    return oss.str();
}

// Create product from file string
Product Product::fromFileString(const string& line) {
    istringstream iss(line);
    string token;

    int id, qty, reorder;
    double price, discount, gst;
    string name, category, brand, description;

    getline(iss, token, ','); id = stoi(token);
    getline(iss, name, ',');
    getline(iss, category, ',');
    getline(iss, brand, ',');
    getline(iss, description, ',');
    getline(iss, token, ','); price = stod(token);
    //getline(iss, token, ','); qty = stoi(token);
    getline(iss, token, ','); discount = stod(token);
    getline(iss, token, ','); gst = stod(token);
    //getline(iss, token, ','); reorder = stoi(token);

    return Product(id, name, category, brand, description, price, discount, gst);
}