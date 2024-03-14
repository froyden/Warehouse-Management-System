#pragma once
#include <string>
#include <vector>

using namespace std;

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
#include "Action.h"

class BaseAction;
class Volunteer;
class Customer;
class SoldierCustomer;

// Warehouse responsible for Volunteers, Customers and Actions.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void processConfigLine(const string& line);
        void cleanMemory();
        // Copy Constructor
        WareHouse(const WareHouse &other);
        // Move Constructor
        WareHouse(WareHouse &&other) noexcept;
        // Copy Assignment Operator
        const WareHouse& operator=(const WareHouse &other);
        // Move Assignment Operator
        WareHouse & operator=(WareHouse &&other);
        // Destructor
        ~WareHouse();
        
        void start();
        const vector<BaseAction*> &getActions() const;
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        void close();
        void open();
        int getCustomerCounter() const;
        int getVolunteerCounter() const;
        int getOrderCounter() const;
        void updateOrderCounter();
        void addCustomer(Customer *customer);
        void AddVolunteer(Volunteer *volunteer);
        void wareHouseSimulateStep();
        Volunteer &findTheNextAvalibleVolunteer(Order& order) const;
        void eraseFromInProccess(int moveOrderId);

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique volunteer IDs
        SoldierCustomer *nullCustomer;
        CollectorVolunteer *nullVolunteer;
        Order *nullOrder;
};