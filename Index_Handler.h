//
// Created by ncast on 3/30/2023.
//

#ifndef FINALPROJEXAMPLES_INDEX_HANDLER_H
#define FINALPROJEXAMPLES_INDEX_HANDLER_H

#include <fstream>
#include "AVLTree.h"
#include <string>
#include <vector>
#include <utility>
#include <chrono>
using namespace chrono;
using namespace std;

class Index_Handler {
private:
    //    word\         file_path\     /frequency
    AVLTree<string,vector<pair<string,int>>> wordTree;
    AVLTree<string,vector<pair<string,int>>> orgTree;
    AVLTree<string,vector<pair<string,int>>> personTree;

    //these values represent the number of unique words in each avl tree, and are set when write_to_persistence() is called
    int uniqueWords=0;
    int uniqueOrgs=0;
    int uniquePersons=0;

public:
    Index_Handler(); //default constructor
    //functions to create avl trees (the index) and save them to a file for later
    void add_to_textTree(const unordered_map<string,pair<string,int>>& text_map);
    void add_to_orgTree(const unordered_map<string,pair<string,int>>& org_map);
    void add_to_personTree(const unordered_map<string,pair<string,int>>& person_map);
    void write_to_persistence(); //calls write_word(), write_org(), and write_person()
    void write_word();          //writes wordTree nodes to a file 
    void write_org();           //writes orgTree nodes to a file
    void write_person();        //writes personTree nodes to a file
    void read_from_persistence();   // calls read_word(), read_org(), and read_person()
    void read_word();               //creates wordTree from text file
    void read_org();                //create orgTree from text file
    void read_person();             //create personTree from text file

    //used to return vectors of data for a certain word
    const vector<pair<string,int>> get_wordData(string key);
    const vector<pair<string,int>> get_orgData(string key);
    const vector<pair<string,int>> get_personData(string key);

    //return other statistics
    const int get_uniqueWords();
    const int get_uniqueOrgs();
    const int get_uniquePersons();
    void resetStats(){
        uniquePersons = 0;
        uniqueOrgs = 0;
        uniqueWords = 0;
    };

    void print_Tree_in_order(); //prints tree in alphabetical order, for debug

    //makes avl trees empty
    void clean_allTrees();
    void clean_wordTree();
    void clean_orgTree();
    void clean_personTree();

};

#endif //FINALPROJEXAMPLES_INDEX_HANDLER_H
