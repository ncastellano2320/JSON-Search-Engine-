//
// Created by ncast on 4/2/2023.
//

#ifndef FINALPROJEXAMPLES_DOCUMENT_PARSER_H
#define FINALPROJEXAMPLES_DOCUMENT_PARSER_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cctype> // for tolower() function
#include <iomanip>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include "Index_Handler.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "porter2_stemmer.h"
using namespace rapidjson;
using namespace std;

class Document_Parser {
private:
    Index_Handler index_handler;
    vector<string> text;   //Vector to hold text in single document
    vector<string> org;    //Vector to hold Organizations in single document
    vector<string> people; //Vector to hold persons in single document
    unordered_map<string,pair<string,int>> text_map;
    unordered_map<string,pair<string,int>> org_map;
    unordered_map<string,pair<string,int>> person_map;
    unordered_set<string> stop_words;


public:
    Document_Parser(); //Constructor, Start, Would populate stopwords and start the parser
    void ReadJsonFile(const string &fileName); //Reads one json file and extracts info needed
    void ReadFileSystem(const string &path);   //Iterator to Go read all files in the folder
    void clean_text(string &text_to_clean);
    void write_to_Persistence();
    void read_From_Persistence();
    void clear_index();
    void print_wordTree_in_order();
    Index_Handler get_Index_Handler(); //return index handler
    void preview_file_data(const string &file_path);
    void single_file_info(const string &file_path);



};

#endif //FINALPROJEXAMPLES_DOCUMENT_PARSER_H
