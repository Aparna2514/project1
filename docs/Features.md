Project: Supply Chain Management System (B2C Model)
This project simulates a supply chain system where a company manages inventory and customers place orders for products. It's built using Object-Oriented Programming in C++ with persistent file storage.

Modules & Features

1. 📂 Product Management (Product Class)
Stores product details: ID, name, brand, price, quantity, category, etc.
Attributes include:
Price
Quantity in stock
Discount percent
GST percent
Reorder threshold
Provides getter/setter methods
Serializable to text files (products.txt)

2. 🧾 Inventory Management (Inventory Class)
Add new product
Update product details
Remove product
View all products
Search products by name
Check and alert low-stock products
Load/save product list from products.txt

3. 👤 Customer Management (Customer Class)
Register new customer
Auto-generated customer ID using Utility::generateID()
Stores:
Name
Email
Address
Phone
Saved in customers.txt
Lookup existing customers
Check if a customer exists by ID

4. 🛒 Order Management (Order Class)
Place a new order
Multiple items allowed
Quantity check against inventory
Updates stock after order placement
Calculates:
Base price
Discounts
GST
Final total

Stores:
Order ID (auto-generated)
Customer ID
Date of order
List of items (product ID & quantity)
Orders saved in orders.txt

5. 🔁 Utility Functions (Utility Class)
Generate unique IDs for:
Products
Customers
Orders
Stores last used ID in id_tracker.txt

💾 File I/O Summary
File Name	Purpose
products.txt	Stores all product data
customers.txt	Stores registered customer info
orders.txt	Logs all customer orders
id_tracker.txt	Tracks and increments unique IDs

🧠 Design Philosophy
Modular design using OOP
Easy to extend (e.g., add GUI or database)
Realistic simulation of a B2C supply chain
Strong separation of concerns

📌 Summary of User Operations
👤 Customer
Register as a new customer
View available products
Place orders

🧑‍💼 Admin
Add/update/delete products
View all or low-stock products
Search inventory
View orders (future enhancement)

