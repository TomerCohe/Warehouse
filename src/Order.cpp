#include "../include/Order.h"
   

 Order::Order(int id, int customerId, int distance):
 id(id) , customerId(customerId), distance(distance),status(OrderStatus::PENDING),
 collectorId(NO_VOLUNTEER),driverId(NO_VOLUNTEER) {}


 int Order::getId() const{
    return id;
 }
    
 int Order::getCustomerId() const{
    return customerId;
 }

 
 int Order::getCollectorId() const{
    return collectorId;
 }

 
 int Order::getDriverId() const{
    return driverId;
 }

 void Order::setStatus(OrderStatus status){
    (*this).status = status;
 }

 void Order::setCollectorId(int collectorId){
    (*this).collectorId = collectorId;
 }

 void Order::setDriverId(int driverId){
    (*this).driverId = driverId;
 }

 OrderStatus Order::getStatus() const{
   return status;
 }

 int Order:: getDistance() const{
   return distance;
 }
 const string Order:: toString() const{
   std::string output;
    if(status == OrderStatus::PENDING){
       output = "OrderId: " + std::to_string(id) + "\n" +
      "OrderStatus: Pending" + "\n" +
      "CustomerId: " + std::to_string(customerId) +"\n" +
      "Collector: None" + "\n" +
      "Driver: None"; 
      }
      else if(status == OrderStatus::COLLECTING){
      output = "OrderId: " + std::to_string(id) + "\n" +
      "OrderStatus: Collecting" + "\n" +
      "CustomerId: " + std::to_string(customerId) +"\n" +
      "Collector: " + std::to_string(collectorId)+ "\n" +
      "Driver: None"; 
      }
      else if(status == OrderStatus::DELIVERING){
      output = "OrderId: " + std::to_string(id) + "\n" +
      "OrderStatus: Delivering" + "\n" +
      "CustomerId: " + std::to_string(customerId) +"\n" +
      "Collector: " + std::to_string(collectorId)+ "\n" +
      "Driver: " + std::to_string(driverId) ; 
      }
      else {
      output = "OrderId: " + std::to_string(id) + "\n" +
      "OrderStatus: Completed" + "\n" +
      "CustomerId: " + std::to_string(customerId) +"\n" +
      "Collector: " + std::to_string(collectorId)+ "\n" +
      "Driver: " + std::to_string(driverId) ;  
      }
      return output;
 }

 Order* Order:: clone() const{
   return new Order(*this);
 }



string Order::orderStatusToString() const{
    if(status == OrderStatus::PENDING)
        return "Pending";
    else if (status == OrderStatus::COLLECTING)
      return "collecting";
    else if (status == OrderStatus::DELIVERING)
      return "delivering";
   
   return "completed";
}
