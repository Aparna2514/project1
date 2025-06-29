#include "Customer.h"
#include "Utility.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <vector>

Customer::Customer(int id, const std::string& name, const std::string& email,
                   const std::string& address, const std::string& phone)
    : customerId(id), name(name), email(email), address(address), phone(phone) {}

// Getters
int Customer::getCustomerId() const { return customerId; }
std::string Customer::getName() const { return name; }
std::string Customer::getEmail() const { return email; }
std::string Customer::getAddress() const { return address; }
std::string Customer::getPhone() const { return phone; }

// Save to file
void Customer::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename, std::ios::app);
    if (outFile) {
        outFile << customerId << "|" << name << "|" << email << "|"
                << address << "|" << phone << "\n";
        outFile.close();
    } else {
        std::cerr << "❌ Unable to open " << filename << " to save customer.\n";
    }
}

// Load from file
Customer Customer::loadFromFile(int id, const std::string& filename) {
    std::ifstream inFile(filename);
    std::string line;
    while (getline(inFile, line)) {
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> parts;

        while (getline(iss, token, '|')) {
            parts.push_back(token);
        }

        if (parts.size() == 5 && std::stoi(parts[0]) == id) {
            return Customer(id, parts[1], parts[2], parts[3], parts[4]);
        }
    }

    std::cerr << "❌ Customer with ID " << id << " not found.\n";
    return Customer(); // empty/default
}

// Check if customer exists
bool Customer::exists(int id, const std::string& filename) {
    std::ifstream inFile(filename);
    std::string line;
    while (getline(inFile, line)) {
        std::istringstream iss(line);
        std::string token;
        if (getline(iss, token, '|') && std::stoi(token) == id) {
            return true;
        }
    }
    return false;
}

// Register a new customer using Utility::generateID
Customer Customer::registerCustomer() {
    std::string name, email, address, phone;

    std::cout << "\n📝 Register New Customer:\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter Name: ";
    std::getline(std::cin, name);

    std::cout << "Enter Email: ";
    std::getline(std::cin, email);

    std::cout << "Enter Address: ";
    std::getline(std::cin, address);

    std::cout << "Enter Phone Number: ";
    std::getline(std::cin, phone);

    int newId = Utility::generateID();

    Customer newCustomer(newId, name, email, address, phone);
    newCustomer.saveToFile();

    std::cout << "✅ Customer registered successfully! Assigned ID: " << newId << "\n";

    return newCustomer;
}
