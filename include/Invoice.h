#ifndef INVOICE_H
#define INVOICE_H

#include "Order.h"
#include "Inventory.h"
#include "Customer.h"

class Invoice {
public:
    static void generateInvoice(const Order& order, const Inventory& inventory, const Customer& customer);
};

#endif // INVOICE_H
