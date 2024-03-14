
#include "Action.h"
#include "WareHouse.h"
#include <iostream>
using std::string;

extern WareHouse* backup; // not sure this goes here

//class Action
BaseAction::BaseAction(): errorMsg(""), status(ActionStatus::ERROR){} 

    ActionStatus BaseAction::getStatus() const{
         return status;
      }

    void BaseAction::complete(){ 
        status = ActionStatus::COMPLETED;
    }

    void BaseAction::error(string errorMsg){
        status = ActionStatus::ERROR;
        this->errorMsg = errorMsg;
        cout<< "Error: " << errorMsg  << endl;
    }

    string BaseAction::getErrorMsg() const{
        return errorMsg;
    }



//class SimulateStep {

        SimulateStep::SimulateStep(int numOfSteps):numOfSteps(numOfSteps){}
        
        void SimulateStep::act(WareHouse &wareHouse) {
            for(int i = 0; i < numOfSteps; i++){
                wareHouse.wareHouseSimulateStep();
            }
            complete();
            wareHouse.addAction(clone());
        }
        std::string SimulateStep::toString() const{
            string completeStatus = (getStatus() == ActionStatus::COMPLETED) ? "COMPLETED" : "ERROR";
            return "simulateStep " + std::to_string(numOfSteps) + " " + completeStatus;                                                 // is this ok??????????????????????????
        }
        SimulateStep *SimulateStep::clone() const{
            return new SimulateStep(*this);
        }



//class AddOrder  {

        AddOrder::AddOrder(int id):customerId(id){}
        
        void AddOrder::act(WareHouse &wareHouse) {
            
            Customer &currentCustomer = wareHouse.getCustomer(customerId);
            if (currentCustomer.getId() == -1){
                error("cannot place this order");
            }
            else{
                int newOrderId = currentCustomer.addOrder(wareHouse.getOrderCounter());
                if (newOrderId == -1){ 
                    error("cannot place this order");
                }
                else {
                    Order *order = new Order(newOrderId, customerId, currentCustomer.getCustomerDistance());
                    wareHouse.addOrder(order);
                    this->complete();
                }
                
            }
            wareHouse.addAction(clone());
        }
    
        
        AddOrder *AddOrder::clone() const {
            return new AddOrder(*this);
        }
        
        
        string AddOrder::toString() const {
            string completeStatus = (getStatus() == ActionStatus::COMPLETED) ? "COMPLETED" : "ERROR";
            return "order " + std::to_string(customerId) + " " + completeStatus;
        }




//class AddCustomer

        AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders):
            customerName(customerName),
            //Given type 'Civilian' if customerType is 'civilian' or 'Soldier' otherwise.
            customerType((customerType == "civilian") ? CustomerType::Civilian : CustomerType::Soldier), 
            distance(distance), maxOrders(maxOrders){}

        
        void AddCustomer::act(WareHouse &wareHouse){
            Customer *currentCustomer = nullptr;
            if (customerType == CustomerType::Civilian){
                currentCustomer = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
            }
            else if (customerType == CustomerType::Soldier){
                currentCustomer = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
            }
            wareHouse.addCustomer(currentCustomer);
            this->complete(); //Only if addition was successful
            wareHouse.addAction(clone());

        }
        
        
        AddCustomer *AddCustomer::clone() const {
            return new AddCustomer(*this);
        }
        
        
        string AddCustomer::toString() const{
            string cusStringType = (customerType == CustomerType::Civilian) ? "Civilian" : "Soldier";
            string completeStatus = (getStatus() == ActionStatus::COMPLETED) ? "COMPLETED" : "ERROR";
            return "customer " + customerName + " " + cusStringType + " " 
                   + std::to_string(distance) + " " + std::to_string(maxOrders) + " " + completeStatus;
        }

    

//class PrintOrderStatus 

  
        PrintOrderStatus::PrintOrderStatus(int id):orderId(id){}


        void PrintOrderStatus::act(WareHouse &wareHouse) {
            Order &currentOrder = wareHouse.getOrder(orderId);
            if (currentOrder.getId() == -1){
                error("Order doesn't exist");
            }
            else{
                std::cout << currentOrder.toString() << endl;
                this->complete(); //Only if print was successful
            }
            wareHouse.addAction(clone());
        }

        PrintOrderStatus *PrintOrderStatus::clone() const {
            return new PrintOrderStatus(*this);                

        }

        string PrintOrderStatus::toString() const {
            string completeStatus = (getStatus() == ActionStatus::COMPLETED) ? "COMPLETED" : "ERROR";
            return "orderStatus " + std::to_string(orderId) + " " + completeStatus;
        }







//class PrintCustomerStatus
   
        PrintCustomerStatus::PrintCustomerStatus(int customerId): customerId(customerId){}
        
        void PrintCustomerStatus::act(WareHouse &wareHouse){
            Customer &currentCustomer = wareHouse.getCustomer(customerId);
            if (currentCustomer.getId() == -1){
                error("Customer doesn't exist");
            }
            else{
                std::cout << "CustomerID: " << customerId << endl;
                for (const auto orderId : currentCustomer.getOrdersIds()){
                    std::cout << wareHouse.getOrder(orderId).toStringIdStatus() << endl;
                }  
                std::cout << "numOrdersLeft: " << currentCustomer.getNumOrdersLeft() << endl;
                this->complete(); //Only if print successful
            }
            wareHouse.addAction(clone());
        }
        
        
        PrintCustomerStatus *PrintCustomerStatus::clone() const{
            return new PrintCustomerStatus(*this);
        }
        
        
        string  PrintCustomerStatus::toString() const{
            string completeStatus = (getStatus() == ActionStatus::COMPLETED) ? "COMPLETED" : "ERROR";
            return "customerStatus " + std::to_string(customerId) + " " + completeStatus;
        }


      
//class PrintVolunteerStatus  

        PrintVolunteerStatus::PrintVolunteerStatus(int id):volunteerId(id){}
        
        void PrintVolunteerStatus::act(WareHouse &wareHouse){
            Volunteer &currentVolunteer = wareHouse.getVolunteer(volunteerId);
            if (currentVolunteer.getId() == -1){
                error("Volunteer doesn't exist");
            }
            else{
                std:: cout << currentVolunteer.toString() << endl;  
                this->complete(); //Only if print successful
            }
            wareHouse.addAction(clone());
        }
        
        PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
            return new PrintVolunteerStatus(*this);
        }
        string PrintVolunteerStatus::toString() const{
            string completeStatus = (getStatus() == ActionStatus::COMPLETED) ? "COMPLETED" : "ERROR";
            return "volunteerStatus " + std::to_string(volunteerId) + " " + completeStatus;
        }
    
     



//class PrintActionsLog 
     
        PrintActionsLog::PrintActionsLog(){}
        
        void PrintActionsLog::act(WareHouse &wareHouse) {
            for (const auto *action : wareHouse.getActions()){
                cout << action->toString() << endl;
            }
            this->complete();
            wareHouse.addAction(clone());
        }

        PrintActionsLog *PrintActionsLog::clone() const {
            return new PrintActionsLog(*this);
            
        }
        string PrintActionsLog::toString() const {
            return "log COMPLETED";
        }




//class Close

        Close::Close(){} // the function finish the program. make sure to free all memory before it ends
        
        void Close::act(WareHouse &wareHouse){
            this->complete();
            wareHouse.addAction(clone());
            wareHouse.close();
        }
        
        Close *Close::clone() const{
            return new Close(*this);
        }
        
        string Close::toString() const{
            return "close COMPLETED";
        }


//class BackupWareHouse 
 
        BackupWareHouse::BackupWareHouse(){}
        
        void BackupWareHouse::act(WareHouse &wareHouse){
            wareHouse.addAction(clone());
            //If this is not the first backup, delete the previous backup
            if (backup != nullptr && backup != &wareHouse){
                delete backup;
                backup = nullptr;
            }
            //If this is the first backup, or same as the previous one - make a copy of the current wareHouse
            if (backup == nullptr){
                backup = new WareHouse(wareHouse);  // Create a copy of the current warehouse
            }
            complete();
            
        }
        BackupWareHouse *BackupWareHouse::clone() const{
            return new BackupWareHouse(*this);
        }
        string BackupWareHouse::toString() const{
            return "backup COMPLETED";
        }



//class RestoreWareHouse 
   
        RestoreWareHouse::RestoreWareHouse(){}
        
        void RestoreWareHouse::act(WareHouse &wareHouse){
            if(backup == nullptr) {
                error("No backup available");
            } else if (backup!= &wareHouse){
                wareHouse = *backup;
                complete();
            }
            wareHouse.addAction(clone());
        }

        RestoreWareHouse *RestoreWareHouse::clone() const{
            return new RestoreWareHouse(*this);

        }

        string RestoreWareHouse::toString() const{
            string completeStatus = (getStatus() == ActionStatus::COMPLETED) ? "COMPLETED" : "ERROR";
            return "restore " + completeStatus;
        }  
