🔹 Overview
This is a Supply Chain Management System built in C++ for a Business-to-Consumer (B2C) model. The system handles the end-to-end flow from managing inventory to handling customer orders and tracking low-stock alerts. It features persistent storage using text files and supports customer registration, product browsing, order placement, and stock monitoring.

✨ Features
🧑‍💼 Admin
Add, update, or remove products from inventory

View all products and low-stock alerts

Search products by name

Save/load product data from files

👤 Customer
Register as a new customer

View available products

Place orders for multiple products

Automatic total calculation including discount and GST

Order saved to file with timestamp and product details


| Class       | Responsibility                                       |
| ----------- | ---------------------------------------------------- |
| `Product`   | Represents a product in inventory                    |
| `Inventory` | Manages stock: add, remove, view, search, update     |
| `Customer`  | Manages customer info & registration                 |
| `Order`     | Manages placing, saving, and calculating orders      |
| `Utility`   | Generates unique IDs for products, customers, orders |


How to Compile & Run
Clone or Download the repository

Make sure data/ folder exists with appropriate .txt files (empty if first run)

Compile using g++:

bash
Copy
Edit
g++ main.cpp Product.cpp Inventory.cpp Customer.cpp Order.cpp Utility.cpp -o scms
Run the executable:

bash
Copy
Edit
./scms

🔐 Dependencies
Pure C++
Standard Library only (no external dependencies)


💡 Future Improvements
Add login/authentication for admin
GUI integration (e.g., using Qt)
File encryption or database integration
Reporting module (total sales, frequent buyers, etc.)

Author Notes
This project is designed for placements, internships, and C++ practice. It focuses on:
OOP design
File handling
Class interaction
Real-world simulation
Clean logic, modular design, and practical implementation make this project great for showcasing C++ skills



