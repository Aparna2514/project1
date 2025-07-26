#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

class Customer {
private:
    int customerId;
    std::string name;
    std::string email;
    std::string address;
    std::string phone;
    std::string pincode;
    std::string password;

public:
    // Constructors
    Customer() = default;
    Customer(int id, const std::string& name, const std::string& email,
             const std::string& address, const std::string& phone,
             const std::string& pincode, const std::string& password);

    // Getters
    int getCustomerId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getAddress() const;
    std::string getPhone() const;
    std::string getPincode() const;
    std::string getPassword() const;

    // File operations
    void saveToFile(const std::string& filename = "data/customers.txt") const;
    static Customer loadFromFile(int id, const std::string& filename = "data/customers.txt");
    static bool exists(int id, const std::string& filename = "data/customers.txt");

    // Core functionality
    static Customer registerCustomer();
    static Customer loginCustomer();

};

#endif // CUSTOMER_H
