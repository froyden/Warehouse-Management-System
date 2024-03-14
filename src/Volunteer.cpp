
#include "Volunteer.h"
#include "Order.h"
#include <iostream>

//class volunteer

//constructor
Volunteer::Volunteer(int id, const string &name):
    completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name){}

    int Volunteer::getId() const {
        return id;
    }

    const string &Volunteer:: getName() const {
        return name;
    }

    int Volunteer::getActiveOrderId() const{
        return activeOrderId;
    }
    
    int Volunteer::getCompletedOrderId() const{
        return completedOrderId;
    }

    bool Volunteer::isBusy() const{
        return activeOrderId != NO_ORDER;
    } 
        
    string Volunteer::orderToString() const{
        if(isBusy()){
            return std::to_string(getActiveOrderId());
        }
        return "None";
    }

    void Volunteer::complete(){
        completedOrderId = NO_ORDER;
    }


//class collectorVolunteer
CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown):
    Volunteer(id, name), coolDown(coolDown), timeLeft(0){}                       //check if the 0 is ok

   
    CollectorVolunteer *CollectorVolunteer :: clone() const {  // i think we may need a copy constructur / copy assingment operator
    // clone = to create copy of the object
    return new CollectorVolunteer(*this);    // may not be right!
    }


    void CollectorVolunteer::step() { //Simulate volunteer step,if the volunteer finished the order, transfer activeOrderId to completedOrderId
        if (activeOrderId != -1 && decreaseCoolDown()){
            completedOrderId = activeOrderId;
            activeOrderId = -1;
        }
    }// in each step, if there is an active order the function update the time that left, if there isn't, the function do nothing
        
    int CollectorVolunteer::getCoolDown() const{
        return coolDown;
    }

    int CollectorVolunteer::getTimeLeft() const{
        return timeLeft;                                
    }
    //returns TRUE iff the collecting is finished
    bool CollectorVolunteer::decreaseCoolDown(){
        if(timeLeft > 0){
            timeLeft = timeLeft - 1;
        }
        return timeLeft == 0;
    }
    
    bool CollectorVolunteer::hasOrdersLeft() const {
        return true;                                     
    }
        
    bool CollectorVolunteer::canTakeOrder(const Order &order) const {
        return !isBusy() && order.getCollectorId() == NO_VOLUNTEER; // the order didnt get a collector yet
    }
    
    void CollectorVolunteer::acceptOrder(const Order &order) { 
        activeOrderId = order.getId();                 
        timeLeft = coolDown;
    }
    
    string CollectorVolunteer::toString() const{
        std::string output = "VolunteerID: " + std::to_string(getId()) + "\n";
        string busy = isBusy()? "True" : "False";
        output += "isBusy: " + busy + "\n";
        output += "Order ID: " + orderToString() + "\n";
        output += "Time Left: " + std::to_string(getTimeLeft())+ "\n";
        output += "OrdersLeft: No Limit \n";
        return output;
    }


    bool CollectorVolunteer::hasFinished() const{
        return getTimeLeft() == 0;
    }




//class LimitedCollectorVolunteer

    LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders):
        CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders){}

        LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const{
            return new LimitedCollectorVolunteer(*this);    
        }

        bool LimitedCollectorVolunteer::hasOrdersLeft() const {
            return ordersLeft > 0;
        }

        bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
            return CollectorVolunteer::canTakeOrder(order) && hasOrdersLeft();
        }

        void  LimitedCollectorVolunteer::decreaseOrdersLeft(){
            ordersLeft = ordersLeft - 1;
        }

        void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
            CollectorVolunteer::acceptOrder(order);
            decreaseOrdersLeft();
        }

        int LimitedCollectorVolunteer::getMaxOrders() const{
            return maxOrders;
        }

        int LimitedCollectorVolunteer::getNumOrdersLeft() const{
            return ordersLeft;
        }

        string LimitedCollectorVolunteer::toString() const {
            std::string output = "VolunteerID: " + std::to_string(getId()) + "\n";
            string busy = isBusy()? "True" : "False";
            output += "isBusy: " + busy + "\n";
            output += "Order ID: " + orderToString() + "\n";
            output += "Time Left: " + std::to_string(getTimeLeft())+ "\n";
            output += "OrdersLeft: "+ std::to_string(getNumOrdersLeft())+" \n";
            return output;
        }


//class DriverVolunteer 


    DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep):
        Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(maxDistance){}

        
        DriverVolunteer *DriverVolunteer::clone() const {
            return new DriverVolunteer(*this); 
        }

        int DriverVolunteer::getDistanceLeft() const{
            return distanceLeft;
        }

        int DriverVolunteer::getMaxDistance() const{
            return maxDistance;
        }
        int DriverVolunteer::getDistancePerStep() const{
            return distancePerStep;
        }
        bool DriverVolunteer::decreaseDistanceLeft(){ //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise            
            distanceLeft = distanceLeft - distancePerStep;
            if (distanceLeft < 0) {
                distanceLeft = 0;
            }
            return distanceLeft == 0;
        }  
        
        bool DriverVolunteer::hasOrdersLeft() const {
            return true;
        }
        
        bool DriverVolunteer::canTakeOrder(const Order &order) const { // Signal if the volunteer is not busy and the order is within the maxDistance
            return (!isBusy() && order.getCollectorId() != NO_VOLUNTEER && order.getDriverId() == NO_VOLUNTEER && order.getDistance() <= maxDistance);  // if the order is waiting for a driver
        } 
        
        void DriverVolunteer::acceptOrder(const Order &order) { // Reset distanceLeft to maxDistance
            activeOrderId = order.getId();
            distanceLeft = order.getDistance();
        } 
        
        void DriverVolunteer::step() { // Decrease distanceLeft by distancePerStep
            if (activeOrderId != -1 && decreaseDistanceLeft()){
                completedOrderId = activeOrderId;
                activeOrderId = -1;
            }
        } 

        
        string DriverVolunteer::toString() const {
            std::string output = "VolunteerID: " + std::to_string(getId()) + "\n";
            string busy = isBusy()? "True" : "False";
            output += "isBusy: " + busy + "\n";
            output += "Order ID: " + orderToString() + "\n";
            output += "Distance Left: " + std::to_string(getDistanceLeft())+ "\n";
            output += "OrdersLeft: No Limit \n";
            return output;
        }

        bool DriverVolunteer::hasFinished() const{
            return  getDistanceLeft()== 0;
    }





//class LimitedDriverVolunteer

   
        LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
                DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders){}

        LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const {
            return new LimitedDriverVolunteer(*this); 
        }
        
        int LimitedDriverVolunteer::getMaxOrders() const{
            return maxOrders;
        }
        
        int LimitedDriverVolunteer::getNumOrdersLeft() const{
            return ordersLeft;
        }
        
        bool LimitedDriverVolunteer::hasOrdersLeft() const {
            return ordersLeft > 0;
        }
        
        bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const { // Signal if the volunteer is not busy, the order is within the maxDistance and have orders left
            return (DriverVolunteer::canTakeOrder(order) && hasOrdersLeft());
        } 
        
        void LimitedDriverVolunteer::acceptOrder(const Order &order) { // Reset distanceLeft to maxDistance and decrease ordersLeft
            DriverVolunteer::acceptOrder(order);
            ordersLeft = ordersLeft - 1;
        } 
        
        string LimitedDriverVolunteer::toString() const {
            std::string output = "VolunteerID: " + std::to_string(getId()) + "\n";
            string busy = isBusy()? "True" : "False";
            output += "isBusy: " + busy + "\n";
            output += "OrderId: " + orderToString() + "\n";
            output += "Distance Left: " + std::to_string(getDistanceLeft())+ "\n";
            output += "OrdersLeft: " + std::to_string(getNumOrdersLeft())+ "\n";
            return output;
        }

        



