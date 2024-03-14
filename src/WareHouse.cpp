
#include "WareHouse.h"
#include "Action.h"
#include "Volunteer.h"
#include "Customer.h"
#include "Order.h"
#include <iostream>
#include <fstream>
#include <sstream>


//class WareHouse 

        WareHouse::WareHouse(const string &configFilePath):     
            isOpen(false), 
            actionsLog(), 
            volunteers(), 
            pendingOrders(),
            inProcessOrders(), 
            completedOrders(), 
            customers(),
            customerCounter(0), 
            volunteerCounter(0), 
            orderCounter(0),
            nullCustomer(new SoldierCustomer(-1, "NullCustomer", -1, -1)), 
            nullVolunteer(new CollectorVolunteer(-1, "NullVolunteer", -1)), 
            nullOrder(new Order(-1, -1, -1)) {
            
            std::ifstream configFile(configFilePath);
            std::string line;
            while (std::getline(configFile, line)) {
                if(!line.empty()){
                    processConfigLine(line);
                }
            }
            configFile.close();
        }

        void WareHouse::processConfigLine(const string& line) {  
            std::istringstream iss(line);
            std::string key;
            iss >> key;
            //Case 1: Adding a Customer
            if(key == "customer") { 
                std::string name, type;
                int distance, maxOrders;
                if (iss >> name >> type >> distance >> maxOrders) {
                    if(type == "soldier"){
                        SoldierCustomer *toAdd = new SoldierCustomer(customerCounter,name,distance,maxOrders);
                        addCustomer(toAdd);
                    } 
                    else if(type == "civilian") {
                        CivilianCustomer *toAdd = new CivilianCustomer(customerCounter,name,distance,maxOrders);
                        addCustomer(toAdd);
                    }
                    else{
                        cout << "Invalid customer type" << endl;
                    }
                } 
                else {
                    std::cout << "Error: Customer not added" << std::endl;
                }
            }

            //Case 2: Adding a volunteer:
            else if (key == "volunteer") {
                std::string name, role;
                int coolDown, maxDistance, distancePerStep, maxOrders;
                iss >> name >> role;
                
                if (role == "collector") {
                    iss >> coolDown;
                    CollectorVolunteer *toAdd= new CollectorVolunteer(volunteerCounter, name, coolDown);
                    AddVolunteer(toAdd);
                } 
                else if (role == "limited_collector") {
                    iss >> coolDown >> maxOrders;
                    LimitedCollectorVolunteer *toAdd = new LimitedCollectorVolunteer(volunteerCounter, name, coolDown, maxOrders);
                    AddVolunteer(toAdd);
                } 
                else if (role == "driver") {
                    iss >> maxDistance >> distancePerStep;
                    DriverVolunteer *toAdd = new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep);
                    AddVolunteer(toAdd);
                } 
                else if (role == "limited_driver") {
                    iss >> maxDistance >> distancePerStep >> maxOrders;
                    LimitedDriverVolunteer *toAdd = new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders);
                    AddVolunteer(toAdd);
                }
                else{
                    std::cout << "the input is not valid" << endl;
                }
            }   
        }

        //copy constructor 
        WareHouse:: WareHouse(const WareHouse &other):
            isOpen(other.isOpen), 
            actionsLog(), 
            volunteers(),
            pendingOrders(), 
            inProcessOrders(), 
            completedOrders(), 
            customers(),
            customerCounter(other.getCustomerCounter()),
            volunteerCounter(other.getVolunteerCounter()),
            orderCounter(other.getOrderCounter()),
            nullCustomer(new SoldierCustomer(-1, "NullCustomer", -1, -1)),
            nullVolunteer(new CollectorVolunteer(-1, "NullCollector", -1)),
            nullOrder(new Order(-1, -1, -1)){
            
            const vector<BaseAction*> & logs = other.getActions();
            for(BaseAction *act: logs){
                actionsLog.push_back(act->clone());
             }
            const vector<Customer*> &otherCustomers = other.customers;
             for(Customer *customer: otherCustomers){
                customers.push_back(customer->clone());
             }
            const vector<Volunteer*> &otherVolunteers = other.volunteers;
            for(Volunteer *volunteer: otherVolunteers){
                volunteers.push_back(volunteer->clone());
             }
            const vector<Order*> &copiedPendingOrders = other.pendingOrders;
              for(Order *order: copiedPendingOrders){
                pendingOrders.push_back(new Order(*order));
             }
            const vector<Order*> &copiedProcessOrders = other.inProcessOrders;
            for(Order *order: copiedProcessOrders){
                inProcessOrders.push_back(new Order(*order));
            }  
            const vector<Order*> &copiedComplitedOrders = other.completedOrders;
            for(Order *order: copiedComplitedOrders){
                completedOrders.push_back(new Order(*order));
            }
        }
        //Move Constructor
        WareHouse:: WareHouse(WareHouse &&other) noexcept:
            isOpen(other.isOpen), 
            actionsLog(), 
            volunteers(),
            pendingOrders(), 
            inProcessOrders(), 
            completedOrders(), 
            customers(),
            customerCounter(other.getCustomerCounter()),
            volunteerCounter(other.getVolunteerCounter()),
            orderCounter(other.getOrderCounter()),
            nullCustomer(new SoldierCustomer(-1, "NullCustomer", -1, -1)),
            nullVolunteer(new CollectorVolunteer(-1, "NullCollector", -1)),
            nullOrder(new Order(-1, -1, -1)){
            actionsLog = std::move(other.actionsLog);
            volunteers = std::move(other.volunteers);
            pendingOrders = std::move(other.pendingOrders);
            inProcessOrders = std::move(other.inProcessOrders);
            completedOrders = std::move(other.completedOrders);
            customers = std::move(other.customers);
            }
            
//Copy Assignment Operator
        const WareHouse& WareHouse::operator=(const WareHouse &other){
            if(this!= &other){
                isOpen = other.isOpen;
                customerCounter = other.getCustomerCounter();
                volunteerCounter = other.getVolunteerCounter();
                orderCounter = other.getOrderCounter();

                for (Order* order : pendingOrders) {
                    delete order;
                }
                pendingOrders.clear();
                for (Order* order : inProcessOrders) {
                    delete order;
                }
                inProcessOrders.clear();
                for (Order* order : completedOrders) {
                    delete order;
                }
                completedOrders.clear(); 
                for (Customer* customer : customers) {
                    delete customer;
                }
                customers.clear();
                for (Volunteer* volunteer : volunteers) {
                    delete volunteer;
                }
                volunteers.clear();
                
                for (BaseAction* action : actionsLog) { 
                    delete action;
                }
                actionsLog.clear();
                for (Customer* customer : other.customers) {
                    customers.push_back(customer->clone());
                }
                for (Volunteer* volunteer : other.volunteers) {
                    volunteers.push_back(volunteer->clone());
                }
                for (Order* order : other.pendingOrders) {
                    pendingOrders.push_back(new Order(*order));
                }
                for (Order* order : other.inProcessOrders) {
                    inProcessOrders.push_back(new Order(*order));
                }
                for (Order* order : other.completedOrders) {
                    completedOrders.push_back(new Order(*order));
                } 
                for (BaseAction* action : other.actionsLog) {
                    actionsLog.push_back(action->clone());
                }
            }
            return *this;
        }
//Move Assignment Operator
        WareHouse & WareHouse::operator=(WareHouse &&other){
            delete nullCustomer;
            delete nullVolunteer;
            delete nullOrder;
            nullCustomer = other.nullCustomer;
            nullVolunteer = other.nullVolunteer;
            nullOrder = other.nullOrder;

            for (Customer* customer : customers) {
                delete customer;
            }
            customers.clear();
            for (Volunteer* volunteer : volunteers) {
                delete volunteer;
            }
            volunteers.clear();
            for (Order* order : pendingOrders) {
                delete order;
            }
            pendingOrders.clear();
            for (Order* order : inProcessOrders) {
                delete order;
            }
            inProcessOrders.clear();
            for (Order* order : completedOrders) {
                delete order;
            }
            completedOrders.clear(); 
            for (BaseAction* action : actionsLog) { 
                delete action;
            }
            actionsLog.clear();
            isOpen = other.isOpen;
            actionsLog = std::move(other.actionsLog);
            pendingOrders = std::move(other.pendingOrders);
            inProcessOrders = std::move(other.inProcessOrders);
            completedOrders = std::move(other.completedOrders);
            customers = std::move(other.customers);
            customerCounter = other.customerCounter;
            volunteerCounter = other.volunteerCounter;
            orderCounter = other.orderCounter;

            return *this;
        }
//DESTRUCTOR
        WareHouse::~WareHouse() {
            delete nullCustomer;
            delete nullVolunteer;
            delete nullOrder;
            nullCustomer = nullptr;
            nullVolunteer = nullptr;
            nullOrder = nullptr;
            
            for (Customer* customer : customers) {
                delete customer;
            }
            customers.clear();
            for (Volunteer* volunteer : volunteers) {
                delete volunteer;
            }
            volunteers.clear();
            for (Order* order : pendingOrders) {
                delete order;
            }
            pendingOrders.clear();
            for (Order* order : inProcessOrders) {
                delete order;
            }
            inProcessOrders.clear();
            for (Order* order : completedOrders) {
                delete order;
            }
            completedOrders.clear(); 
            for (BaseAction* action : actionsLog) {
                delete action;
            }
            actionsLog.clear();
        }      
//START FUNCTION
        void WareHouse::start(){ 
            open();
            std:: cout << "Warehouse is Open!" << endl;
            while (isOpen) {
                std::string userInput;
                std::cout << "Enter an action: ";
                std::getline(std::cin, userInput);
                std::istringstream iss(userInput);
                std::string actionType;
                iss >> actionType;

                if (actionType == "step") {
                    int numOfSteps;
                    if(iss >> numOfSteps){          
                        SimulateStep action = SimulateStep(numOfSteps);
                        action.act(*this);
                    } else {
                        std::cout << "Invalid input" << std::endl;
                    }
                }
                else if (actionType == "order") {
                    int customerId;
                    if(iss >> customerId) {
                        AddOrder action = AddOrder(customerId);
                        action.act(*this);
                    } else {
                        std::cout << "Invalid input" << std::endl;
                    }    
                } 
                else if (actionType == "customer") {
                    std::string customerName, customerType;
                    int distance, maxOrders;
                    if(iss >> customerName >> customerType >> distance >> maxOrders) {
                        AddCustomer action = AddCustomer(customerName, customerType, distance, maxOrders);
                        action.act(*this);

                    } 
                    else {
                        std::cout << "Invalid input" << std::endl;
                    }
                } 
                else if (actionType == "orderStatus") {
                    int orderId;
                    if(iss >> orderId) {
                        PrintOrderStatus action = PrintOrderStatus(orderId);
                        action.act(*this);
                    } else {
                        std::cout << "Invalid input" << std::endl;
                    }
                } 
                else if (actionType == "customerStatus") {
                    int customerId;
                    if(iss >> customerId) {
                        PrintCustomerStatus action = PrintCustomerStatus(customerId);
                        action.act(*this);
                    } else {
                        std::cout << "Invalid input" << std::endl;
                    }
                } 
                else if (actionType == "volunteerStatus") {
                    int volunteerId;
                    if(iss >> volunteerId){
                        PrintVolunteerStatus action = PrintVolunteerStatus(volunteerId);
                        action.act(*this);
                    } else {
                        std::cout << "Invalid input" << std::endl;
                    }
                } 
                else if (actionType == "log") {
                    PrintActionsLog action = PrintActionsLog();
                    action.act(*this);
                } 
                else if (actionType == "close") {
                    Close action = Close();
                    action.act(*this);
                } 
                else if (actionType == "backup") {
                    BackupWareHouse action = BackupWareHouse();
                    action.act(*this);
                } 
                else if (actionType == "restore") {
                    RestoreWareHouse action = RestoreWareHouse();
                    action.act(*this);
                } 
                else {
                    std:: cout << "Unknown action type" << endl;
                }
            }
        }
//get Actions Log
        const vector<BaseAction*> &WareHouse::getActions() const{
            return actionsLog;
        }
//Add Order to Pending Orders      
        void WareHouse::addOrder(Order* order){
            pendingOrders.push_back(order);
            updateOrderCounter();
        }
//Add Action to Actions Log
        void WareHouse::addAction(BaseAction* action){
            actionsLog.push_back(action);
        }
//Get Customer by customerId        
        Customer &WareHouse::getCustomer(int customerId) const{
            for (const auto &customer : customers){
                if(customer->getId() == customerId){
                    return *customer;                        
                }
            }
            return *nullCustomer;
        }
 //Get Volunteer by ID       
        Volunteer &WareHouse::getVolunteer(int volunteerId) const{
            for (const auto &volunteer : volunteers){
                if(volunteer->getId() == volunteerId){
                    return *volunteer;                        
                }
            }
            return *nullVolunteer;
        }
//Get Order by ID       
        Order &WareHouse::getOrder(int orderId) const{
            for (const auto &order : pendingOrders){
                if(order->getId()== orderId){
                    return *order;                        
                } 
            }
            for (const auto &order : inProcessOrders){
                if(order->getId() == orderId){
                    return *order;                        
                }
            }
            for (const auto &order : completedOrders){
                if(order->getId() == orderId){
                    return *order;                             
                }
            }
            return *nullOrder;

        }
//Close WareHouse
        void WareHouse::close(){
            for(const auto *order: pendingOrders){
                std::cout<< order->toStringClose()<<endl;
            }
            for(const auto *order: inProcessOrders){
                std::cout<< order->toStringClose()<<endl;
            }
            for(const auto *order: completedOrders){
                std::cout<< order->toStringClose()<<endl;
            }
            isOpen = false;
        }
//Open WareHouse        
        void WareHouse::open(){
            isOpen = true;
        }
//Get Customer Counter       
        int WareHouse::getCustomerCounter() const{
            return customerCounter;
        }
//Get Volunteer Counter      
        int WareHouse::getVolunteerCounter() const{
            return volunteerCounter;
        }
//Get Order Counter       
        int WareHouse::getOrderCounter() const{
            return orderCounter;
        }
//Increment Order Counter       
        void WareHouse::updateOrderCounter(){
            orderCounter = orderCounter + 1;
        }
//Add Customer to customers
        void WareHouse::addCustomer(Customer *customer){
            customers.push_back(customer);
            customerCounter = customerCounter + 1;
        }

//Add Volunteer to volunteers
        void WareHouse::AddVolunteer(Volunteer *volunteer){
            volunteers.push_back(volunteer);
            volunteerCounter = volunteerCounter + 1;
         }
//Simulate 1 Step for warehouse
        void WareHouse::wareHouseSimulateStep(){
            //Goes through all pending orders
            for (auto currentOrder = pendingOrders.begin(); currentOrder != pendingOrders.end();) {
                Volunteer *currentVolunteer= nullptr;
                currentVolunteer= &findTheNextAvalibleVolunteer(**currentOrder);
                // if we found an available volunteer:
                if(currentVolunteer != nullptr && currentVolunteer->getName() != "NullVolunteer"){ 
                    currentVolunteer->acceptOrder(**currentOrder);
                    (*currentOrder)->orderSimulateStep(currentVolunteer->getId());     // the order changes it status and put in the field the id of the volunteer.
                    inProcessOrders.push_back(*currentOrder);       // if it gets here the function is erasing the order and moving to the next one so we dont need order++
                    currentOrder = pendingOrders.erase(currentOrder);
                }
                else{
                    currentOrder++;
                }
            }
            //Goes through all volunteers, takes a step for each volunteer and updates orders accordingly
            for (auto volunteer = volunteers.begin(); volunteer != volunteers.end();) {
                (*volunteer)->step();
                //If volunteer has finished his order:
                if ((*volunteer)->hasFinished() && (*volunteer)->getCompletedOrderId() != NO_ORDER) {
                    Order &order = getOrder((*volunteer)->getCompletedOrderId());
                    if (order.getStatus() == OrderStatus::COLLECTING) { // stay collecting untill we find driver
                        eraseFromInProccess(order.getId());
                        pendingOrders.push_back(&order);                
                        
                    } 
                    else if (order.getStatus() == OrderStatus::DELIVERING) {
                        order.orderSimulateStep(NO_VOLUNTEER); // move to COMPLETE with no id 
                        eraseFromInProccess(order.getId());
                        pendingOrders.push_back(&order);                
                                                                                                      
                    }
                    (*volunteer)->complete();  // complete id = -1 again
                    
                    if (!(*volunteer)->hasOrdersLeft()) { // if the volunteer is limited and no orders left
                        delete (*volunteer);
                        volunteer = volunteers.erase(volunteer);
                        
                    } 
                    else {
                        ++volunteer;
                    }
                } 
                else {
                    ++volunteer;
                }
            }
        }
//finds the next available volunteer for the order
        Volunteer &WareHouse::findTheNextAvalibleVolunteer(Order &order) const {
            for(const auto volunteer: volunteers){
                if (volunteer->canTakeOrder(order)){
                    return *volunteer;
                }
            }
            return *nullVolunteer;
         }
//Erase Order from inProcess orders
        void WareHouse::eraseFromInProccess(int moveOrderId) {
            for (auto it = inProcessOrders.begin(); it != inProcessOrders.end();)  {
                if ((*it)->getId() == moveOrderId) {
                    inProcessOrders.erase(it);
                    return;
                }
                ++it;
            }
        }

  

  
  











