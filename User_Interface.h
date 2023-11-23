//
// Created by ncast on 4/2/2023.
//
#ifndef FINALPROJEXAMPLES_USER_INTERFACE_H
#define FINALPROJEXAMPLES_USER_INTERFACE_H
#include <iostream>
#include "Document_Parser.h"
#include "Index_Handler.h"
#include "Query_Processor.h"
#include <chrono>
using namespace std;
using namespace std::chrono;


class User_Interface {
private:
    Document_Parser documentParser;

public:
    User_Interface();
    void start();
    void runDocParser();
    void createPersistance();
    void readPersistance();
    void clearIndex();
    void query_search();
    void stats();
    

};

#endif //FINALPROJEXAMPLES_USER_INTERFACE_H
