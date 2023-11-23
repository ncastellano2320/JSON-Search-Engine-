//
// Created by ncast on 4/2/2023.
//
#include "Query_Processor.h"

vector<pair<string, int>> Query_Processor::set_intersect(const vector<pair<string, int>> &word1, const vector<pair<string, int>> &word2) {
    vector<pair<string, int>> intersect;
    //do set intersection but add the frequencies if comparison is found
    size_t i = 0, j = 0;
    while (i < word1.size() && j < word2.size()) {
        if (word1[i].first == word2[j].first) {
            intersect.emplace_back(word1[i].first, word1[i].second + word2[j].second);
            ++i;
            ++j;
        } else if (word1[i].first < word2[j].first) {
            ++i;
        } else {
            ++j;
        }
    }
    return intersect;
}

vector<pair<string,int>> Query_Processor::set_difference(const vector<pair<std::string, int>> &intersect, const vector<pair<std::string, int>> &bad_word) {

    // Initialize iterators to point to the beginning of the vectors
    auto it1 = intersect.begin();
    auto it2 = bad_word.begin();

    // Create a vector to store the difference
    std::vector<std::pair<std::string, int>> difference;

    // Iterate through the vectors
    while (it1 != intersect.end() && it2 != bad_word.end()) {
        if (it1->first < it2->first) {
            // Pair from vec1 is not in vec2
            difference.push_back(*it1);
            ++it1;
        }
        else if (it1->first > it2->first) {
            // Pair from vec2 is not in vec1
            ++it2;
        }
        else {
            // Pairs have same key
            ++it1;
            ++it2;
        }
    }

    // Add any remaining pairs from vec1
    while (it1 != intersect.end()) {
        difference.push_back(*it1);
        ++it1;
    }

    return difference;
}

void Query_Processor::sort_by_docs() {
    //Custom comparison operator to sort by strings in pairs
    auto comp = [](const pair<string,int>&a,const pair<string,int>& b) {return a.first < b.first;};

    if (!query_words_vectors.empty()) {
        for (auto& vec: query_words_vectors) {
            sort(vec.begin(), vec.end(), comp);
        }

        for (auto& vec: query_excluded_vector) {
            sort(vec.begin(), vec.end(), comp);
        }
    }

}

void Query_Processor::find_final_intersection() {
    // Initialize intersect vector with the first inner vector
    vector<pair<string, int>> the_intersect = query_words_vectors[0];

    if (query_words_vectors.size() > 1) {
        // Iterate through the remaining inner vectors
        for (size_t vec_index = 1; vec_index < query_words_vectors.size(); vec_index++) {
            // Create a temporary vector to store the intersection
            vector<pair<string, int>> tempIntersect;
            // Find the intersection of the current inner vector and the intersect vector
            tempIntersect = set_intersect(the_intersect,query_words_vectors[vec_index]);

            // Update intersect vector with the result of the intersection
            the_intersect = tempIntersect;
        }
    }

    final_intersect = the_intersect;
    if (!excluded_Terms.empty()) {
        //Find Difference with however many words that were found with a - in front of them in query
        for (const auto& index: query_excluded_vector) {
            //check to see if in results
            final_intersect = set_difference(final_intersect,index);
        }
    }

    //Custom comparison operator to sort by strings in ints for relevancy
    auto comp = [](const pair<string, int> &a, const pair<string, int> &b) { return a.second < b.second; };

    //sort the final intersect so it can be in order for relevancy
    sort(final_intersect.begin(), final_intersect.end(), comp);
}

Query_Processor::Query_Processor(string &query, Index_Handler &indexHandler1) {
    //Set Index_Handler
    indexHandler = indexHandler1;
    //Example Query: "words to searching -data ORG:Apple PERSON:bob"
    //Counter to keep track of number of terms in query for all search terms including orgs and persons and a counter for excluded terms as well
    size_t search_count = 0;
    size_t excluded_count = 0;
    // Split the query by whitespace
    query.push_back(' '); //add space to work with the find(" ");
    size_t start = 0;
    size_t end = query.find(' ');
    while (end != string::npos) {
        string token = query.substr(start, end - start);

        if (token[0] == '-') {
            excluded_count++;
            // Exclude next token from search
            string excludedTerm = query.substr(start + 1, end - start - 1);
            // Convert the string to lowercase
            for (char& c : excludedTerm) {
                c = std::tolower(c);
            }
            //Stemming
            Porter2Stemmer::trim(excludedTerm);
            Porter2Stemmer::stem(excludedTerm);
            excluded_Terms.push_back(excludedTerm);
            start = end;

        } else if (token.substr(0, 4) == "ORG:") {
            search_count++;
            // Extract organization name from "ORG:" prefix
            string orgName = token.substr(4);
            // Convert the string to lowercase
            for (char& c : orgName) {
                c = std::tolower(c);
            }
            query_orgs.push_back(orgName);
            start = end;

        } else if (token.substr(0, 7) == "PERSON:") {
            search_count++;
            // Extract person name from "PERSON:" prefix
            string personName = token.substr(7);
            // Convert the string to lowercase
            for (char& c : personName) {
                c = std::tolower(c);
            }
            query_persons.push_back(personName);
            start = end;

        } else {
            search_count++;
            // Convert the string to lowercase
            for (char& c : token) {
                c = std::tolower(c);
            }
            //Stemming
            Porter2Stemmer::trim(token);
            Porter2Stemmer::stem(token);
            // Add search term
            search_terms.push_back(token);
        }

        start = end + 1;
        end = query.find(' ', start + 1);
    }

    //fill the query_words_vector and query_excluded_vector
    fill_query_words();

    //Find Intersect if all search terms were found in index not looking for -words
    if (query_words_vectors.size() == search_count) {
        find_final_intersection();
        if (final_intersect.empty()) {
            cout << "No Documents found containing terms: " << query << endl;
        }
    } else {
        cout << "No Documents found containing terms: " << query << endl;
    }



}

void Query_Processor::fill_query_words() {
    for (const auto& index: search_terms) {
        vector<pair<string,int>> temp = indexHandler.get_wordData(index);
        //Adds to the query vector if temp is not empty meaning something was found in tree
        if (!temp.empty()) {
            query_words_vectors.push_back(temp);
        }
    }

    for (const auto& index: query_orgs) {
        vector<pair<string,int>> temp = indexHandler.get_orgData(index);
        if (!temp.empty()) {
            query_words_vectors.push_back(temp);
        }
    }

    for (const auto& index: query_persons) {
        vector<pair<string,int>> temp = indexHandler.get_personData(index);
        if (!temp.empty()) {
            query_words_vectors.push_back(temp);
        }
    }

    for (const auto& index: excluded_Terms) {
        vector<pair<string,int>> temp = indexHandler.get_wordData(index);
        if (!temp.empty()) {
            query_excluded_vector.push_back(temp);
        }
    }

    //sort the query_words_vector and query_excluded_vector
    sort_by_docs();

}

vector<pair<string,int>> Query_Processor::return_final_intersection(){
    
    return final_intersect;
    
}