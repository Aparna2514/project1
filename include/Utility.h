#ifndef UTILITY_H
#define UTILITY_H

#include <string>

class Utility {
public:
    static int generateID();                         // For products, orders, customers
    static std::string getCurrentTimestamp();        // For invoices
    static std::string trim(const std::string& str); // (Optional) cleanup input
};

#endif // UTILITY_H
