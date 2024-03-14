#pragma once
#include <string>
#include <vector>
#include "WareHouse.h"
using std::string;
using std::vector;
class WareHouse;


enum class ActionStatus{
    COMPLETED, ERROR
};

enum class CustomerType{
    Soldier, Civilian
};


class Customer;

class BaseAction{        // after each action is complete is it complete succesfully the method complete() should work. else, the methos error() and update the error msg
    public:
        BaseAction();
        virtual ~BaseAction()=default;
        ActionStatus getStatus() const;
        virtual void act(WareHouse& wareHouse)=0;
        virtual string toString() const=0;
        virtual BaseAction* clone() const=0;
        

    protected:
        void complete();
        void error(string errorMsg);
        string getErrorMsg() const;

    private:
        string errorMsg;
        ActionStatus status;
        
};

class SimulateStep : public BaseAction {   // update volunteer's maxOrders right after associating with new order

    public:
        SimulateStep(int numOfSteps);
        ~SimulateStep() override = default;
        void act(WareHouse &wareHouse) override;
        std::string toString() const override;
        SimulateStep *clone() const override;
        

    private:
        const int numOfSteps;
};

class AddOrder : public BaseAction {
    public:
        AddOrder(int id);
        ~AddOrder() override = default;
        void act(WareHouse &wareHouse) override;
        AddOrder *clone() const override;
        string toString() const override;
        
    private:
        const int customerId;
};


class AddCustomer : public BaseAction {
    public:
        AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders);
        ~AddCustomer() override = default;
        void act(WareHouse &wareHouse) override;
        AddCustomer *clone() const override;
        string toString() const override;
    private:
        const string customerName;
        const CustomerType customerType;
        const int distance;
        const int maxOrders;
};



class PrintOrderStatus : public BaseAction {
    public:
        PrintOrderStatus(int id);
        ~PrintOrderStatus() override = default;
        void act(WareHouse &wareHouse) override;
        PrintOrderStatus *clone() const override;
        string toString() const override;
    private:
        const int orderId;
};

class PrintCustomerStatus : public BaseAction {
    public:
        PrintCustomerStatus(int customerId);
        ~PrintCustomerStatus() override = default;
        void act(WareHouse &wareHouse) override;
        PrintCustomerStatus *clone() const override;
        string toString() const override;
    private:
        const int customerId;
};


class PrintVolunteerStatus : public BaseAction {  
    public:
        PrintVolunteerStatus(int id);
        ~PrintVolunteerStatus() override = default;
        void act(WareHouse &wareHouse) override;
        PrintVolunteerStatus *clone() const override;
        string toString() const override;
    private:
        const int volunteerId;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        ~PrintActionsLog() override = default;
        void act(WareHouse &wareHouse) override;
        PrintActionsLog *clone() const override;
        string toString() const override;
    private:
};

class Close : public BaseAction {  
    public:
        Close();
        ~Close() override = default;
        void act(WareHouse &wareHouse) override;
        Close *clone() const override;
        string toString() const override;
    private:
};

class BackupWareHouse : public BaseAction {
    public:
        BackupWareHouse();
        ~BackupWareHouse() override = default;
        void act(WareHouse &wareHouse) override;
        BackupWareHouse *clone() const override;
        string toString() const override;
    private:
};


class RestoreWareHouse : public BaseAction {
    public:
        RestoreWareHouse();
        ~RestoreWareHouse() override = default;
        void act(WareHouse &wareHouse) override;
        RestoreWareHouse *clone() const override;
        string toString() const override;
    private:
};