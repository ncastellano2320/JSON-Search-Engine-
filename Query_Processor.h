//
// Created by ncast on 4/2/2023.
//
#ifndef FINALPROJEXAMPLES_QUERY_PROCESSOR_H
#define FINALPROJEXAMPLES_QUERY_PROCESSOR_H
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include "Index_Handler.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "porter2_stemmer.h"

using namespace std;

class Query_Processor {
private:
    Index_Handler indexHandler; //Used to access the AVL Trees
    vector<string> search_terms; //Cleaned words from query
    vector<string> query_persons; //persons form query
    vector<string> query_orgs; //orgs from query
    vector<string> excluded_Terms; //words to exclude followed by -
    vector<vector<pair<string,int>>> query_excluded_vector; //The vector of pairs
    vector<vector<pair<string,int>>> query_words_vectors; //The vectors of pairs {Doc, frequency} for each query word,org,and person
    vector<pair<string,int>> final_intersect; //Overlap in vectors of pairs with only documents that contain all words in query also excluding words that had an - in front of them
    static vector<pair<string,int>> set_intersect(const vector<pair<string, int>>& word1, const vector<pair<string, int>>& word2); //Custom function to find intersection between vectors of pairs
    static vector<pair<string,int>> set_difference(const vector<pair<string, int>>& word1, const vector<pair<string, int>>& word2);
public:
    Query_Processor(string &user_query, Index_Handler &indexHandler1);
    void fill_query_words();
    void sort_by_docs(); //Sort all the vectors in the query_words_vectors and query_excluded_vector by the strings for finding intersection
    void find_final_intersection(); //Iterate through query_words_vectors to find final intersection between all
    vector<pair<string,int>> return_final_intersection(); //returns final_intersect
};

#endif //FINALPROJEXAMPLES_QUERY_PROCESSOR_H
