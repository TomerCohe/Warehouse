#include "../include/Action.h"


ActionStatus BaseAction:: getStatus() const{
    return status;
}

void BaseAction:: complete(){
    status = ActionStatus::COMPLETED;
}

void BaseAction:: error(string errorMsg){
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

string BaseAction:: getErrorMsg() const{
    return errorMsg;
}

string BaseAction:: actionStatusToString() const{
    if(status == ActionStatus::COMPLETED)
        return "COMPLETED";
    return "ERROR";
}

BaseAction:: BaseAction():errorMsg(""), status(ActionStatus::ERROR){}



//-----------------------------------------------------------------------

AddOrder::AddOrder(int id): //id here refers to the customer's ID
customerId(id){
    error("Cannot make order");
}

void AddOrder:: act(WareHouse &wareHouse){
    auto& customer = wareHouse.getCustomer(customerId);
    if(customer.getId() != -1 && customer.canMakeOrder()){
        Order* order = new Order(wareHouse.getOrderCounter(),customerId,customer.getCustomerDistance());
        wareHouse.addOrder(order);
        customer.addOrder(order->getId());
        complete();
    }
    else{
        std::cout << getErrorMsg() << std::endl;
    }
}

string AddOrder:: toString() const{
     return "order " +std::to_string(customerId) + " " + actionStatusToString();
}


AddOrder* AddOrder:: clone() const{
    return new AddOrder(*this);
}


//----------------------------------------------------------------------


CustomerType AddCustomer::typeDecide(string customerType){
    if(customerType == "civilian" || customerType == "Civilian")
        return CustomerType::Civilian;
    return CustomerType::Soldier;
}

string AddCustomer::printType() const{
    if(customerType == CustomerType::Civilian)
        return "civilian";
    return "soldier";
}

AddCustomer:: AddCustomer(string customerName, string customerType, int distance, int maxOrders):
customerName(customerName),customerType(typeDecide(customerType)),distance(distance),maxOrders(maxOrders){}


string AddCustomer:: toString() const{
    return "customer " + customerName + " " + printType() + " " + std::to_string(distance) + " " + std::to_string(maxOrders) + " " + actionStatusToString();
}

AddCustomer * AddCustomer:: clone() const{
    return new AddCustomer(*this);
}

void AddCustomer:: act(WareHouse &wareHouse){
    Customer* customer;
    if(customerType == CustomerType::Civilian){
        customer = new CivilianCustomer(wareHouse.getCustomerCounter(),customerName,distance,maxOrders);
    }
    else{
        customer = new SoldierCustomer(wareHouse.getCustomerCounter(),customerName,distance,maxOrders);
    }
    wareHouse.addCustomer(customer);
    complete();
}

//---------------------------------------------------------------------------------------------------


PrintOrderStatus::PrintOrderStatus(int id):
orderId(id){
    error("Order doesn’t exist");
}

void PrintOrderStatus:: act(WareHouse &wareHouse){
    Order& order = wareHouse.getOrder(orderId);
    if(order.getId() == -1){
        std::cout << getErrorMsg() << std::endl;
    }
    else{
        std::cout << order.toString() << std::endl;
        complete();
    }
}


PrintOrderStatus* PrintOrderStatus:: clone() const{
    return new PrintOrderStatus(*this);
}


string PrintOrderStatus:: toString() const{
    return "orderStatus " + std::to_string(orderId) + " " + actionStatusToString();
} 

//----------------------------------------------------------------------------------------------------


PrintCustomerStatus:: PrintCustomerStatus(int customerId):
customerId(customerId){
    error("Customer Doesn't exist");
}

void PrintCustomerStatus:: act(WareHouse &wareHouse){
    Customer& customer = wareHouse.getCustomer(customerId);
    if(customer.getId() == -1){
        std::cout <<getErrorMsg()<<std::endl;
    }
    else{
        std::cout <<"CustomerId: " <<customerId<<std::endl;
        for(int orderId:customer.getOrdersIds()){
            std::cout <<"OrderId: " <<orderId<<std::endl;
            std::cout <<"OrderStatus: " <<wareHouse.getOrder(orderId).orderStatusToString()<<std::endl;           
        }
        std::cout <<"numOrdersLeft: " <<(customer.getMaxOrders() - customer.getNumOrders())<<std::endl;
        complete();
    }

}

PrintCustomerStatus* PrintCustomerStatus:: clone() const{
    return new PrintCustomerStatus(*this);
}


string PrintCustomerStatus:: toString() const{
    return "customerStatus " + std::to_string(customerId) + " " + actionStatusToString();
} 


//------------------------------------------------------------------------------------------------

PrintVolunteerStatus:: PrintVolunteerStatus(int id):
VolunteerId(id){
    error("Volunteer doesn't exist");
}

void PrintVolunteerStatus:: act(WareHouse &wareHouse){
    auto& volunteer = wareHouse.getVolunteer(VolunteerId);
    if(volunteer.getId() == -1){
        std::cout <<getErrorMsg()<<std::endl;
    }
    else{
        std::cout<< volunteer.toString() << std::endl;
        complete();
    }

}

PrintVolunteerStatus* PrintVolunteerStatus:: clone() const{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus:: toString() const{
    return "volunteerStatus " + std::to_string(VolunteerId) + " " + actionStatusToString();
} 



//---------------------------------------------------------------------------------------------------

PrintActionsLog:: PrintActionsLog(){}


void PrintActionsLog:: act(WareHouse &wareHouse){
    for(auto action:wareHouse.getActions()){
        std::cout<<action->toString()<<std::endl;
    }
    complete();
}


PrintActionsLog* PrintActionsLog:: clone() const{
    return new PrintActionsLog(*this);
}

string PrintActionsLog:: toString() const{
    return "log " + actionStatusToString();
} 

//----------------------------------------------------------------------------------------


SimulateStep:: SimulateStep(int numOfSteps):
numOfSteps(numOfSteps){}


void SimulateStep:: act(WareHouse &wareHouse){
    for(int i = 0 ; i < numOfSteps ; i++){
        wareHouse.stageOne();
        wareHouse.stageTwo();
    }
    complete();
}

SimulateStep* SimulateStep:: clone() const{
    return new SimulateStep(*this);
}

string SimulateStep:: toString() const{
    return "simulateStep " + std::to_string(numOfSteps) + " " + actionStatusToString();
} 


//---------------------------------------------------------------------------------------

Close:: Close(){}

void Close:: act(WareHouse &wareHouse){
    wareHouse.close();
    complete();
}

Close* Close:: clone() const{
    return new Close(*this);
}

string Close:: toString() const{
    return "close " + actionStatusToString();
}


//--------------------------------------------------------------------------

BackupWareHouse:: BackupWareHouse(){}

void BackupWareHouse:: act(WareHouse &wareHouse){
    if(backup != nullptr)
        delete backup;
    backup = new WareHouse(wareHouse);
    complete();
}

BackupWareHouse* BackupWareHouse:: clone() const{
    return new BackupWareHouse(*this);
}

string BackupWareHouse:: toString() const{
    return "backup " + actionStatusToString();
}

//--------------------------------------------------------------------------

RestoreWareHouse:: RestoreWareHouse(){
    error("No backup available");
}

void RestoreWareHouse:: act(WareHouse &wareHouse){
    if(backup != nullptr){
        wareHouse = (*backup);
        complete();
    }
    else{
        std::cout <<getErrorMsg()<<std::endl;
    }
}


RestoreWareHouse* RestoreWareHouse:: clone() const{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse:: toString() const{
    return "restore " + actionStatusToString();
}

