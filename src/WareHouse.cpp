#include "../include/WareHouse.h"
#include "../include/Action.h"


WareHouse:: WareHouse(const string &configFilePath):
isOpen(true),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(0),volunteerCounter(0),orderCounter(0),
FAKEVOLUNTEER(new CollectorVolunteer(-1,"fake",-1)),FAKECUSTOMER(new CivilianCustomer(-1,"fake",-1,-1)),FAKEORDER(new Order(-1,-1,-1)){
    std::ifstream inputFile(configFilePath);
    if(!(inputFile.is_open())){
        std::cout <<"File opening error" << std::endl;
    }
    
    std::string line;
    while(std::getline(inputFile,line)){
        std::istringstream iss(line);
        std::string role;
        if(iss >> role){
            if(role == "customer" || role == "Customer"){
                std::string name,type;
                int distance,maxorders;
                Customer* customer;
                if(iss >> name >> type >> distance >> maxorders){
                    if(type == "Civilian" || type == "civilian"){
                        customer = new CivilianCustomer(customerCounter,name,distance,maxorders);
                        customerCounter++;
                        customers.push_back(customer);
                    }
                    else if(type == "Soldier" || type == "soldier"){
                        customer = new SoldierCustomer(customerCounter,name,distance,maxorders);
                        customerCounter++;
                        customers.push_back(customer);
                    }
                    else std::cout<< "illegal customer type "<< type <<std::endl ;
                }
            }
            else if(role == "volunteer" || role == "Volunteer"){
                std::string name,type;
                if(iss >> name >> type){
                    if(type == "collector" ||type == "Collector"){
                        int cooldown;
                        if(iss >> cooldown){
                            CollectorVolunteer* cvol = new CollectorVolunteer(volunteerCounter,name,cooldown);
                            volunteerCounter++;
                            volunteers.push_back(cvol);
                        }
                    }

                    else if(type == "limited_collector"||type == "Limited_Collector"){
                        int cooldown,maxorders;
                        if(iss >> cooldown >> maxorders){
                            LimitedCollectorVolunteer* lcvol = new LimitedCollectorVolunteer(volunteerCounter,name,cooldown,maxorders);
                            volunteerCounter++;
                            volunteers.push_back(lcvol);
                        }

                    }

                    else if(type == "driver"||type == "Driver"){
                        int maxdistance,distanceperstep;
                        if(iss >> maxdistance >> distanceperstep){
                            DriverVolunteer* dvol = new DriverVolunteer(volunteerCounter,name,maxdistance,distanceperstep);
                            volunteerCounter++;
                            volunteers.push_back(dvol);
                        }
                    }

                    else if(type == "limited_driver"||type == "Limited_Driver"){
                        int maxdistance,distanceperstep,maxorders;
                        if(iss >> maxdistance >> distanceperstep >> maxorders){
                            LimitedDriverVolunteer* ldvol = new LimitedDriverVolunteer(volunteerCounter,name,maxdistance,distanceperstep,maxorders);
                            volunteerCounter++;
                            volunteers.push_back(ldvol);
                        }

                    }

                    else std::cout<< "illegal volunteer type "<< type <<std::endl ;

                }

            }
            else std::cout<< "illegal role "<<role<<std::endl ;
        
        }
    }
    inputFile.close();


}



void WareHouse:: start(){
    open();
    while(isOpen){
        string command;
        std::getline(std::cin,command);
        istringstream iss(command);
        std::string action;
        BaseAction* todo;
        bool legalAction = true;
        if(iss>>action){
            if(action == "step"){
                int arg1;
                if(iss>>arg1){
                    todo = new SimulateStep(arg1);
                }
            }
            else if(action == "order"){
                int arg1;
                if(iss>>arg1){
                    todo = new AddOrder(arg1);
                }
            }
            else if(action == "customer"){
                string arg1,arg2;
                int arg3,arg4;
                if(iss>>arg1>>arg2>>arg3>>arg4){
                    todo = new AddCustomer(arg1,arg2,arg3,arg4);
                }
            }
            else if(action == "orderStatus"){
                int arg1;
                if(iss>>arg1){
                    todo = new PrintOrderStatus(arg1);
                }
            }
            else if(action == "customerStatus"){
                int arg1;
                if(iss>>arg1){
                    todo = new PrintCustomerStatus(arg1);
                }
            }
            else if(action == "volunteerStatus"){
                int arg1;
                if(iss>>arg1){
                    todo = new PrintVolunteerStatus(arg1);
                }
            }
            else if(action == "log"){
                todo = new PrintActionsLog();
            }
            else if(action == "close"){
                todo = new Close();
            }
            else if(action == "backup"){
                todo = new BackupWareHouse();
            }
            else if(action == "restore"){
                todo = new RestoreWareHouse();
            }
            else{
                legalAction = false;
                std::cout<<"illegal action"<<std::endl;
            }
            if(legalAction){
                (*todo).act(*this);
                actionsLog.push_back(todo);
            }
            
        std::cin.clear();
        }
        
    }
}

void WareHouse:: addOrder(Order* order){
    pendingOrders.push_back(order);
    orderCounter++;
}

void WareHouse:: addAction(BaseAction* action){
    actionsLog.push_back(action);
}

Customer& WareHouse:: getCustomer(int customerId) const{
    for(Customer* customer:customers){
        if(customer->getId() == customerId) 
            return (*customer);

    }
    
    return (*FAKECUSTOMER);
}

Volunteer& WareHouse:: getVolunteer(int volunteerId) const{
    for(Volunteer* volunteer:volunteers){
        if(volunteer->getId() == volunteerId) 
            return (*volunteer);

    }
    return (*FAKEVOLUNTEER);
}

Order& WareHouse:: getOrder(int orderId) const{
    for(Order* order:pendingOrders){
        if(order->getId() == orderId) 
            return (*order);
    }

    for(Order* order:inProcessOrders){
        if(order->getId() == orderId) 
            return (*order);
    }  

    for(Order* order:completedOrders){
        if(order->getId() == orderId) 
            return (*order);
    }

    return (*FAKEORDER);
}

const vector<BaseAction*> & WareHouse:: getActions() const{
    return actionsLog;
}

void WareHouse::close(){
    for(Order* order:pendingOrders){
        std::cout<<"Order Id: "<<order->getId()<<", Customer Id:"<<order->getCustomerId()<<", Order Status: "<<order->orderStatusToString()<<std::endl;
    }
    for(Order* order:inProcessOrders){
        std::cout<<"Order Id: "<<order->getId()<<", Customer Id:"<<order->getCustomerId()<<", Order Status: "<<order->orderStatusToString()<<std::endl;
    }
    for(Order* order:completedOrders){
        std::cout<<"Order Id: "<<order->getId()<<", Customer Id:"<<order->getCustomerId()<<", Order Status: "<<order->orderStatusToString()<<std::endl;
    }

    
    isOpen = false;

}

void WareHouse:: open(){
    std::cout<<"Warehouse is open"<<std::endl;
    isOpen = true;
}

WareHouse::WareHouse(const WareHouse &other):
isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter),
FAKEVOLUNTEER(new CollectorVolunteer(-1,"fake",-1)),FAKECUSTOMER(new CivilianCustomer(-1,"fake",-1,-1)),FAKEORDER(new Order(-1,-1,-1)){
    for(Volunteer* volunteer:other.volunteers){
        volunteers.push_back((*volunteer).clone());
    }
    for(Customer* customer:other.customers){
        customers.push_back((*customer).clone());
    }
    for(Order* order:other.pendingOrders){
        pendingOrders.push_back((*order).clone());
    }
    for(Order* order:other.inProcessOrders){
        inProcessOrders.push_back((*order).clone());
    }
    for(Order* order:other.completedOrders){
        completedOrders.push_back((*order).clone());
    }

    for(BaseAction* action:other.actionsLog){
        actionsLog.push_back((*action).clone()); //check again after finishing action.cpp
    }
}   


WareHouse:: WareHouse(WareHouse &&other):
isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter),
FAKEVOLUNTEER(new CollectorVolunteer(-1,"fake",-1)),FAKECUSTOMER(new CivilianCustomer(-1,"fake",-1,-1)),FAKEORDER(new Order(-1,-1,-1)){
    actionsLog = std::move(other.actionsLog);
    volunteers = std::move(other.volunteers);
    pendingOrders = std::move(other.pendingOrders);
    inProcessOrders = std::move(other.inProcessOrders);
    completedOrders = std::move(other.completedOrders);
    customers = std::move(other.customers);
    
    
    other.isOpen = false;  
    other.customerCounter = 0;
    other.volunteerCounter = 0;
    other.orderCounter = 0;
    other.actionsLog.clear();
    other.volunteers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.customers.clear();
}


 WareHouse& WareHouse:: operator=(const WareHouse &other){
    if(this != &other){
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        delete FAKECUSTOMER;
        delete FAKEVOLUNTEER;
        delete FAKEORDER;

        FAKECUSTOMER = new CivilianCustomer(-1,"fake",-1,-1);
        FAKEVOLUNTEER = new DriverVolunteer(-1,"fake",-1,-1);
        FAKEORDER = new Order(-1,-1,-1);


        for(Volunteer* volunteer:volunteers){
            delete volunteer;
        }
        volunteers.clear();
        for(Volunteer* volunteer:other.volunteers){
            volunteers.push_back((*volunteer).clone());
        }

        for(Customer* customer:customers){
            delete customer;
        }
        customers.clear();
        for(Customer* customer:other.customers){
            customers.push_back((*customer).clone());
        }

        for(Order* order:pendingOrders){
            delete order;
        }
        pendingOrders.clear();
        for(Order* order:other.pendingOrders){
            pendingOrders.push_back((*order).clone());
        }

        for(Order* order:inProcessOrders){
            delete order;
        }
        inProcessOrders.clear();
        for(Order* order:other.inProcessOrders){
            inProcessOrders.push_back((*order).clone());
        }

        for(Order* order:completedOrders){
            delete order;
        }
        completedOrders.clear();
        for(Order* order:other.completedOrders){
            completedOrders.push_back((*order).clone());
        }

        for(BaseAction* action:actionsLog){
            delete action;
        }
        actionsLog.clear();
        for(BaseAction* action:other.actionsLog){
            actionsLog.push_back((*action).clone()); //recheck
        }
    }
    return *this;

 }

WareHouse& WareHouse:: operator=(WareHouse &&other){
    if (this != &other) {  // self-assignment check
        for(Volunteer* volunteer:volunteers){
            delete volunteer;
        }
        volunteers.clear();
            
        for(Customer* customer:customers){
            delete customer;
        }
        customers.clear();

        for(Order* order:pendingOrders){
            delete order;
        }
        pendingOrders.clear();

        for(Order* order:inProcessOrders){
            delete order;
        }
        inProcessOrders.clear();

        for(Order* order:completedOrders){
            delete order;
        }
        completedOrders.clear();

        for(BaseAction* action:actionsLog){
            delete action;
        }
        actionsLog.clear();

        isOpen = other.isOpen;
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        delete FAKECUSTOMER;
        delete FAKEVOLUNTEER;
        delete FAKEORDER;

        FAKECUSTOMER = other.FAKECUSTOMER;
        FAKEVOLUNTEER = other.FAKEVOLUNTEER;
        FAKEORDER = other.FAKEORDER;

        other.FAKECUSTOMER = nullptr;
        other.FAKEVOLUNTEER = nullptr;
        other.FAKEORDER = nullptr;


        other.isOpen = false;  
        other.customerCounter = 0;
        other.volunteerCounter = 0;
        other.orderCounter = 0;
        other.actionsLog.clear();
        other.volunteers.clear();
        other.pendingOrders.clear();
        other.inProcessOrders.clear();
        other.completedOrders.clear();
        other.customers.clear();
    }
    return *this;
}


WareHouse::~WareHouse(){
    for(Volunteer* volunteer:volunteers){
            delete volunteer;
        }
    volunteers.clear();

    for(Customer* customer:customers){
            delete customer;
        }
    customers.clear();

    for(Order* order:pendingOrders){
            delete order;
    }
    pendingOrders.clear();
    
    for(Order* order:inProcessOrders){
        delete order;
    }
    inProcessOrders.clear();

    for(Order* order:completedOrders){
            delete order;
    }
    completedOrders.clear();

    for(BaseAction* action:actionsLog){
            delete action;
    }
    actionsLog.clear();

    delete FAKECUSTOMER;
    delete FAKEVOLUNTEER;
    delete FAKEORDER;

}


int WareHouse:: getOrderCounter() const{
    return orderCounter;
}


void WareHouse:: addCustomer(Customer* customer){
    customers.push_back(customer);
    customerCounter++;
}

int WareHouse:: getCustomerCounter() const{
    return customerCounter;
}


// for simulatestep()

void WareHouse:: stageOne(){
    for(Volunteer* volunteer:volunteers){
        if(!(volunteer->isBusy())){
            if(CollectorVolunteer* collectorVolunteer = dynamic_cast<CollectorVolunteer*>(volunteer)){
               for(auto order = pendingOrders.begin();order!=pendingOrders.end();order++){
                    if((*order)->getStatus() == OrderStatus::PENDING){
                        collectorVolunteer->acceptOrder(**order); //inv:if collector is not busy he can take an order otherwise we would have deleted him
                        (*order)->setStatus(OrderStatus::COLLECTING);
                        (*order)->setCollectorId(collectorVolunteer->getId());
                        inProcessOrders.push_back(*order);
                        pendingOrders.erase(order);
                        break;
                    }
               }
            }
            else{
                for(auto order = pendingOrders.begin();order!=pendingOrders.end();order++){
                    if((*order)->getStatus() == OrderStatus::COLLECTING && volunteer->canTakeOrder(**order)){
                        volunteer->acceptOrder(**order); //inv:if collector is not busy he can take an order otherwise we would have deleted him
                        (*order)->setStatus(OrderStatus::DELIVERING);
                        (*order)->setDriverId(volunteer->getId());
                        inProcessOrders.push_back(*order);
                        pendingOrders.erase(order);
                        break;
                    }
               }
            }
               
        }
    }
}


void WareHouse:: stageTwo(){
    for(auto volunteer = volunteers.begin();volunteer!=volunteers.end();volunteer++){
        if((*volunteer)->isBusy()){
            (*volunteer)->step();
            if(!(*volunteer)->isBusy()){
                Order& doneOrder = getOrder((*volunteer)->getCompletedOrderId());
                if(doneOrder.getStatus()==OrderStatus::COLLECTING){
                    pendingOrders.push_back(&doneOrder);
                    bool erased = false;
                    for(auto it = inProcessOrders.begin();it != inProcessOrders.end() && (erased == false);it++){
                        if(*it == &doneOrder){
                            inProcessOrders.erase(it);
                            erased = true;
                        }
                    }
                }
                else{
                    completedOrders.push_back(&doneOrder);
                    bool erased = false;
                    for(auto it = inProcessOrders.begin();it != inProcessOrders.end() && (erased == false);it++){
                        if(*it == &doneOrder){
                            inProcessOrders.erase(it);
                            erased = true;
                        }
                    }
                    doneOrder.setStatus(OrderStatus::COMPLETED); 
                }
                if(!(*volunteer)->hasOrdersLeft()){
                    delete (*volunteer);
                    volunteers.erase(volunteer);
                }
            }
        }
    }
}

