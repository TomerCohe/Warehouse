#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h" //added manually

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        //rule of 5
        WareHouse(const WareHouse &other);
        WareHouse(WareHouse &&other);

        WareHouse& operator=(const WareHouse &other);
        WareHouse& operator=(WareHouse &&other);

        ~WareHouse();

        // ------------------------------------------------------


        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        int getOrderCounter() const;
        void addCustomer(Customer* customer);
        int getCustomerCounter() const;
        
        //functions for simulate step
        void stageOne();
        void stageTwo();
        

        

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique order IDs
        Volunteer* FAKEVOLUNTEER;
        Customer* FAKECUSTOMER;
        Order* FAKEORDER;


};