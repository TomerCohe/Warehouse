#include "../include/Volunteer.h"

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown):
Volunteer(id,name),coolDown(coolDown),timeLeft(NO_ORDER){} //what to initialize timeleft as

CollectorVolunteer* CollectorVolunteer::clone() const{ //might need re-do
    return new CollectorVolunteer(*this);
}

bool CollectorVolunteer:: decreaseCoolDown(){
    timeLeft--;
    return timeLeft == 0;
}

void CollectorVolunteer::step(){
    if(isBusy()){
        if(decreaseCoolDown()){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}

int CollectorVolunteer:: getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer:: getTimeLeft() const{
    return timeLeft;
}


bool CollectorVolunteer::hasOrdersLeft() const{
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const{  //doesn't depend on the specific order we get
    return (hasOrdersLeft() && !isBusy());
}

void CollectorVolunteer::acceptOrder(const Order &order){
    //if(canTakeOrder(order)){
        activeOrderId = order.getId();
        timeLeft = coolDown;
    //}
}

string CollectorVolunteer::toString() const {
    std::string output = 
    "VolunteerId: " + std::to_string(getId()) + "\n" +
    "IsBusy: " ;
    if(isBusy()){
        output+= "True \nOrderId: " + std::to_string(activeOrderId) + "\n" +
        "timeLeft: " + std::to_string(timeLeft);
    }
    else{
        output+="False \nOrderId: None \ntimeLeft: None"; 
    }
    output+= "\nOrdersLeft: No limit";
    return output;
}

 void CollectorVolunteer:: setTimeLeft(int timeLeft){
    (*this).timeLeft = timeLeft;
}

//LimitedCollectorVolunteer:

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders):
CollectorVolunteer(id,name,coolDown), maxOrders(maxOrders),ordersLeft(maxOrders){}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer:: hasOrdersLeft() const{
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer :: canTakeOrder(const Order &order) const{
    return (hasOrdersLeft() && !isBusy());
}

void LimitedCollectorVolunteer:: acceptOrder(const Order &order){
    //if(canTakeOrder(order)){
        activeOrderId = order.getId();
        setTimeLeft(getCoolDown());
        ordersLeft--;
    //}
}

string LimitedCollectorVolunteer:: toString() const{
    std::string output = 
    "VolunteerId: " + std::to_string(getId()) + "\n" +
    "IsBusy : " ;
    if(isBusy()){
        output+= "True \nOrderId: " + std::to_string(activeOrderId) + "\n" +
        "timeLeft: " + std::to_string(getTimeLeft());
    }
    else{
        output+="False \nOrderId: None \ntimeLeft: None"; 
    }
    output+= "\nOrdersLeft: " + std::to_string(ordersLeft);
    return output;
}