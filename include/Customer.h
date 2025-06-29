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

public:
    // Constructors
    Customer() = default;
    Customer(int id, const std::string& name, const std::string& email,
             const std::string& address, const std::string& phone);

    // Getters
    int getCustomerId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getAddress() const;
    std::string getPhone() const;

    // File operations
    void saveToFile(const std::string& filename = "data/customers.txt") const;
    static Customer loadFromFile(int id, const std::string& filename = "data/customers.txt");
    static bool exists(int id, const std::string& filename = "data/customers.txt");

    // Register new customer (user input based)
    static Customer registerCustomer();
};

#endif // CUSTOMER_H


