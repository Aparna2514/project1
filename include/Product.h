#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
private:
    int productId;
    std::string name;
    std::string category;
    std::string brand;
    std::string description;
    double price;
    int quantity;
    double discountPercent;
    double gstPercent;
    int reorderThreshold;

public:
    // Constructor
    Product(int id, const std::string& name, const std::string& category,
            const std::string& brand, const std::string& description,
            double price, int quantity, double discount, double gst, int reorderThreshold);

    // Getters
    int getProductId() const;
    std::string getName() const;
    std::string getCategory() const;
    std::string getBrand() const;
    std::string getDescription() const;
    double getPrice() const;
    int getQuantity() const;
    double getDiscountPercent() const;
    double getGstPercent() const;
    int getReorderThreshold() const;

    // Setters
    void setName(const std::string&);
    void setCategory(const std::string&);
    void setBrand(const std::string&);
    void setDescription(const std::string&);
    void setPrice(double);
    void setQuantity(int);
    void setDiscountPercent(double);
    void setGstPercent(double);
    void setReorderThreshold(int);

    // Display
    void displayProduct() const;
    void displayForCustomer() const;

    // Inventory-related
    void updateQuantity(int change);
    bool isLowStock() const;

    // Pricing
    double getDiscountedPrice() const;
    double getFinalPriceWithGST() const;

    // File I/O support
    std::string toFileString() const;
    static Product fromFileString(const std::string& line);
};

#endif // PRODUCT_H
