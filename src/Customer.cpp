#include "../include/Customer.h"

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders):
id(id),name(name),locationDistance(locationDistance),maxOrders(maxOrders),ordersId(), numOrders(0) {

}

const string& Customer::getName() const{
    return name;
}

int Customer::getId() const{
    return id;
}

int Customer::getCustomerDistance() const{
    return locationDistance;
}

int Customer::getMaxOrders() const{
    return maxOrders;
}

int Customer::getNumOrders() const{
    return numOrders;
}

bool Customer::canMakeOrder() const{
    return (numOrders < maxOrders);
}

const vector<int>& Customer:: getOrdersIds() const{
    return ordersId;
}

int Customer::addOrder(int orderId){
    if(canMakeOrder()){
        numOrders++;
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;

}

SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders):
Customer(id,name,locationDistance,maxOrders){}

SoldierCustomer* SoldierCustomer::clone() const{  //might need re-do
      return new SoldierCustomer(*this);
    }

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders):
Customer(id,name,locationDistance,maxOrders){}

CivilianCustomer* CivilianCustomer::clone() const{  //might need re-do
    
    return  new CivilianCustomer(*this);
}

