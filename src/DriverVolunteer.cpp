#include "../include/Volunteer.h"

DriverVolunteer:: DriverVolunteer(int id, string name, int maxDistance, int distancePerStep):
Volunteer(id,name),maxDistance(maxDistance),distancePerStep(distancePerStep),distanceLeft(NO_ORDER){}

DriverVolunteer * DriverVolunteer::clone() const{
    return  new DriverVolunteer(*this);
}

int DriverVolunteer:: getDistanceLeft() const{
    return distanceLeft;
}

int DriverVolunteer:: getMaxDistance() const{
    return maxDistance;
}

int DriverVolunteer:: getDistancePerStep() const{
    return distancePerStep;
}

bool DriverVolunteer:: decreaseDistanceLeft(){
    distanceLeft-=distancePerStep;
    if(distanceLeft<=0){
        distanceLeft=0;
        return true;
    }
    return false;
}

bool DriverVolunteer:: hasOrdersLeft() const{
    return true;
}

bool DriverVolunteer:: canTakeOrder(const Order &order) const{
    return (hasOrdersLeft()&&!isBusy()&& order.getDistance()<=maxDistance);
}

void DriverVolunteer:: acceptOrder(const Order &order){
    if(canTakeOrder(order)){
        distanceLeft = order.getDistance();
        activeOrderId = order.getId();
    }
}

void DriverVolunteer:: step(){
    if(isBusy()){
        if(decreaseDistanceLeft()){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}

void DriverVolunteer:: setDistanceLeft(int distanceLeft){
    (*this).distanceLeft = distanceLeft;
}


string DriverVolunteer:: toString() const{
    std::string output = 
    "VolunteerId: " + std::to_string(getId()) + "\n" +
    "IsBusy:" ;
    if(isBusy()){
        output+= std::to_string(activeOrderId) + "\n" +
        "DistanceLeft: " + std::to_string(distanceLeft);
    }
    else{
        output+="None \nDistanceLeft: None"; 
    }
    output+= "\nOrdersLeft: No limit";
    return output;
}



LimitedDriverVolunteer:: LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
DriverVolunteer(id,name,maxDistance,distancePerStep),maxOrders(maxOrders),ordersLeft(maxOrders){}


LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer:: getMaxOrders() const{
    return maxOrders;
}

int LimitedDriverVolunteer:: getNumOrdersLeft() const{
    return ordersLeft;
}

bool LimitedDriverVolunteer:: hasOrdersLeft() const{
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer:: canTakeOrder(const Order &order) const{
    return (hasOrdersLeft()&&!isBusy()&& order.getDistance()<=getMaxDistance() &&hasOrdersLeft());
}

void LimitedDriverVolunteer:: acceptOrder(const Order &order){
    if(canTakeOrder(order)){
        setDistanceLeft(order.getDistance());
        activeOrderId = order.getId();
        ordersLeft--;
    }
}

string LimitedDriverVolunteer:: toString() const{
    std::string output = 
    "VolunteerId: " + std::to_string(getId()) + "\n" +
    "IsBusy:" ;
    if(isBusy()){
        output+= std::to_string(activeOrderId) + "\n" +
        "DistanceLeft: " + std::to_string(getDistanceLeft());
    }
    else{
        output+="None \nDistanceLeft: None"; 
    }
    output+= "\nOrdersLeft: " + std::to_string(ordersLeft);
    return output;
}