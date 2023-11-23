//
// Created by ncast on 4/2/2023.
//
#ifndef FINALPROJEXAMPLES_HASH_MAP_H
#define FINALPROJEXAMPLES_HASH_MAP_H
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>
using namespace std;

//Hashmap to implement {organization/person, vector<pair<doc name, frequency>>}

// Define the hash map class
template<typename KeyType, typename ValueType>
class HashMap {
private:
    static const size_t hashgroups;
    list<pair<KeyType,ValueType>> *table; // List 1. Index 0, List 2. Index 1...
public:
    HashMap() {
        static const size_t hashgroups = 1000;
        table = new list<pair<KeyType,ValueType>> [hashgroups]; // List 1. Index 0, List 2. Index 1...
    }
    ~HashMap() {
        delete [] table;
    }
    bool isEmpty() const;
    bool contains(KeyType key);
    size_t hashFunction(KeyType key);
    void insertItem(KeyType key, ValueType value);
    ValueType get(KeyType key);
    void clear();

};



#endif //FINALPROJEXAMPLES_HASH_MAP_H
