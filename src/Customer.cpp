
#include "Customer.h"
#include <iostream>

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders):
    id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId(){}

    int Customer::getId() const {
        return id;
    }

    const string & Customer::getName() const {
        return name;
    }

    int Customer::getCustomerDistance() const {
        return locationDistance;
    }
    
    int Customer::getMaxOrders() const {
        return maxOrders;
    }

    int Customer::getNumOrders() const {
        return ordersId.size();      
    }

    int Customer::getNumOrdersLeft() const{
        return getMaxOrders() - getNumOrders();
    }

    bool Customer::canMakeOrder() const {
        return maxOrders > getNumOrders();       
    }

    const vector<int> & Customer::getOrdersIds() const {
        return ordersId;        
    }

    int Customer::addOrder(int orderId){
        if (canMakeOrder()){
            ordersId.push_back(orderId);
            return orderId;
        }
        return -1;
    }


//class SoldierCustomer
  
    SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders):
        Customer(id, name, locationDistance, maxOrders){}

    SoldierCustomer *SoldierCustomer::clone() const {
        return new SoldierCustomer(*this);
        }

    string SoldierCustomer::toString() const {
        std::string output = "CustomerID: " + std::to_string(getId()) + "\n";
        output += "Name:" + (getName()) + "\n";
        output += "Type: Soldier\n";
        output += "Location Distance: " + std::to_string(getCustomerDistance()) + "\n";
        output += "max orders: " + std::to_string(getMaxOrders())+ "\n";
        output += "OrdersID:";
        const std::vector<int>& orderIds = getOrdersIds();
            for (size_t i = 0; i < orderIds.size(); ++i) {
                output += std::to_string(orderIds[i]) + " ";
            }

        return output;
    }
    

    
 
//class CivilianCustomer
  
        CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders):
            Customer(id, name, locationDistance, maxOrders){}

        
        CivilianCustomer *CivilianCustomer::clone() const {
            return new CivilianCustomer(*this);
        }

        string CivilianCustomer::toString() const {
            std::string output = "CustomerID: " + std::to_string(getId()) + "\n";
            output += "Name:" + (getName()) + "\n"; 
            output += "Type: Civilian\n";
            output += "Location Distance: " + std::to_string(getCustomerDistance()) + "\n";
            output += "max orders: " + std::to_string(getMaxOrders())+ "\n";
            output += "OrdersID:";
            const std::vector<int>& orderIds = getOrdersIds();
                for (size_t i = 0; i < orderIds.size(); ++i) {
                    output += std::to_string(orderIds[i]) + " ";
                }

            return output;
        }

    
  
        

