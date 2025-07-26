#include "Customer.h"
#include "Utility.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <vector>
#include <regex>

// Constructor
Customer::Customer(int id, const std::string& name, const std::string& email,
                   const std::string& address, const std::string& phone,
                   const std::string& pincode, const std::string& password)
    : customerId(id), name(name), email(email), address(address),
      phone(phone), pincode(pincode), password(password) {}

// Getters
int Customer::getCustomerId() const { return customerId; }
std::string Customer::getName() const { return name; }
std::string Customer::getEmail() const { return email; }
std::string Customer::getAddress() const { return address; }
std::string Customer::getPhone() const { return phone; }
std::string Customer::getPincode() const { return pincode; }
std::string Customer::getPassword() const { return password; }

// Save to file
void Customer::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename, std::ios::app);
    if (outFile) {
        outFile << customerId << "|" << name << "|" << email << "|"
                << address << "|" << phone << "|" << pincode << "|"
                << password << "\n";
        outFile.close();
    } else {
        std::cerr << "Unable to open " << filename << " to save customer.\n";
    }
}

// Load from file
Customer Customer::loadFromFile(int id, const std::string& filename) {
    std::ifstream inFile(filename);
    std::string line;
    while (getline(inFile, line)) {
        std::istringstream iss(line);
        std::vector<std::string> parts;
        std::string token;
        while (getline(iss, token, '|')) {
            parts.push_back(token);
        }
        if (parts.size() == 7 && std::stoi(parts[0]) == id) {
            return Customer(id, parts[1], parts[2], parts[3], parts[4], parts[5], parts[6]);
        }
    }
    std::cerr << "Customer with ID " << id << " not found.\n";
    return Customer();
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

// Register a new customer
Customer Customer::registerCustomer() {
    std::string name, email, address, phone, pincode, password;

    std::cout << "\n--- Register New Customer ---\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter Name: ";
    std::getline(std::cin, name);

    std::cout << "Enter Email: ";
    std::getline(std::cin, email);

    std::cout << "Enter Address: ";
    std::getline(std::cin, address);

    std::cout << "Enter Phone Number: ";
    std::getline(std::cin, phone);

    while (true) {
        std::cout << "Enter 6-digit Pincode: ";
        std::getline(std::cin, pincode);

        if (std::regex_match(pincode, std::regex("^[0-9]{6}$"))) break;
        else std::cout << "Invalid pincode. Try again.\n";
    }

    std::cout << "Set a Password: ";
    std::getline(std::cin, password);

    int newId = Utility::generateID();

    Customer newCustomer(newId, name, email, address, phone, pincode, password);
    newCustomer.saveToFile();

    std::cout << "\nCustomer registered successfully! Your ID: " << newId << "\n";

    return newCustomer;
}

// Login for existing customer
Customer Customer::loginCustomer() {
    int idAttempt;
    std::string passAttempt;
    int attempts = 0;

    while (true) {
        std::cout << "\n--- Customer Login ---\n";
        std::cout << "Enter Customer ID (or 0 to return to menu): ";
        std::cin >> idAttempt;

        if (idAttempt == 0) {
            std::cout << "Returning to main menu...\n";
            return Customer();  // Return empty
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter Password: ";
        std::getline(std::cin, passAttempt);

        std::ifstream inFile("data/customers.txt");
        std::string line;
        bool idFound = false;

        while (getline(inFile, line)) {
            std::istringstream iss(line);
            std::vector<std::string> parts;
            std::string token;

            while (getline(iss, token, '|')) {
                parts.push_back(token);
            }

            if (parts.size() == 7 && std::stoi(parts[0]) == idAttempt) {
                idFound = true;
                if (parts[6] == passAttempt) {
                    std::cout << "Login successful. Welcome back, " << parts[1] << "!\n";
                    return Customer(idAttempt, parts[1], parts[2], parts[3], parts[4], parts[5], parts[6]);
                } else {
                    std::cerr << "Incorrect password.\n";
                    break;
                }
            }
        }

        if (!idFound) {
            std::cerr << "Customer ID not found.\n";
        }

        attempts++;
        if (attempts >= 3) {
            std::cerr << "Too many failed attempts. Returning to menu.\n";
            return Customer();
        }

        std::cout << "Try again (" << (3 - attempts) << " attempts left)...\n";
    }
}


