
#include "Order.h"
#include <iostream>

Order::Order(int id, int customerId, int distance):
    id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER){}

    int Order::getId() const {
        return id;
    }


    int Order::getCustomerId() const {
        return customerId;
    }


    int Order::getDistance() const {
        return distance;
    }

    int Order::getCollectorId() const {
        return collectorId;
    }

    int Order::getDriverId() const {
        return driverId;
    }

    OrderStatus Order::getStatus() const {
        return status;
    }

    void Order::setStatus(OrderStatus newStatus) {
        status = newStatus;
    }

    void Order::setCollectorId(int collectorId) {
        this->collectorId = collectorId;
    }

    void Order::setDriverId(int driverId) {
        this->driverId = driverId;
    }

    const std::string orderStatusToString(OrderStatus status){ 
        if(status == OrderStatus::PENDING) {return "Pending";}
        if (status == OrderStatus::COLLECTING) {return "Collecting";}
        if (status == OrderStatus::DELIVERING) {return "Delivering";}
        if (status == OrderStatus::COMPLETED) {return "Completed";}
        return "Unkown";
    }

    const std::string CollectorDriverToString(int id){ 
        if(id == NO_VOLUNTEER){
            return "None";
        }
        return std::to_string(id);
    }


    const std::string Order::toString() const{
        std::string output = "orderId: " + std::to_string(getId()) + "\n";
        output += "OrderStatus: " + orderStatusToString(status) + "\n";
        output += "CustomerId: " + std::to_string(customerId) + "\n";
        output += "Collector: " + CollectorDriverToString(collectorId) + "\n";
        output += "Driver: " + CollectorDriverToString(driverId) + "\n";
        return output;
    }


    const std::string Order::toStringIdStatus() const{
    std::string output = "orderId: " + std::to_string(getId()) + "\n";
    output += "OrderStatus: " + std::string(orderStatusToString(status));
    return output;
    }


    const std::string Order::toStringClose() const{
    std::string output = "orderID: " + std::to_string(getId()) + ", ";
    output += "CustomerID: " + std::to_string(getCustomerId())+ ", ";
    output += "OrderStatus: " + std::string(orderStatusToString(status));
    return output;
    }


    void Order::orderSimulateStep(int id){
        if (getStatus() == OrderStatus::PENDING){
            setStatus(OrderStatus::COLLECTING);
            setCollectorId(id);
        }
        else if (getStatus() == OrderStatus::COLLECTING){
            setStatus(OrderStatus::DELIVERING);
            setDriverId(id);
        }
        else if (getStatus() == OrderStatus::DELIVERING){
            setStatus(OrderStatus::COMPLETED);
        }
     }



