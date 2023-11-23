//
// Created by ncast on 3/30/2023.
//
#include "Index_Handler.h"

Index_Handler::Index_Handler(){};

void Index_Handler::add_to_textTree(const unordered_map<string, pair<string, int>>& text_map) {
    for (const auto& index : text_map) {
        // cout<< index.first << endl;
        vector<pair<string,int>> temp_vec;
        temp_vec.push_back(index.second);
        pair<string,vector<pair<string,int>>> temp(index.first,temp_vec);
        if (!wordTree.search(index.first)) {
            uniqueWords++;
            wordTree.insert(temp);
        } else {
            wordTree.find(index.first)->second.push_back(index.second);  //add a new pair, document and frequency
        }

        temp_vec.clear();
    }
}

void Index_Handler::add_to_orgTree(const unordered_map<string, pair<string, int>>& org_map) {
    for (const auto& index : org_map) {
        // cout<< index.first << endl;
        vector<pair<string,int>> temp_vec;
        temp_vec.push_back(index.second);
        pair<string,vector<pair<string,int>>> temp(index.first,temp_vec);
        if (!orgTree.search(index.first)) {
            orgTree.insert(temp);
            uniqueOrgs++;
        } else {
            orgTree.find(index.first)->second.push_back(index.second);  //add a new pair, document and frequency
        }

        temp_vec.clear();
    }
}

void Index_Handler::add_to_personTree(const unordered_map<string, pair<string, int>>& person_map) {
    for (const auto& index : person_map) {
        // cout<< index.first << endl;
        vector<pair<string,int>> temp_vec;
        temp_vec.push_back(index.second);
        pair<string,vector<pair<string,int>>> temp(index.first,temp_vec);
        if (!personTree.search(index.first)) {
            personTree.insert(temp);
            uniquePersons++;
        } else {
            personTree.find(index.first)->second.push_back(index.second);  //add a new pair, document and frequency
        }

        temp_vec.clear();
    }
}

void Index_Handler::read_from_persistence(){ //reads from permenent tree file, and recreates an AVL tree
    auto startPersistence = high_resolution_clock::now(); //start chrono timer to track runtime
    read_word();
    read_org();
    read_person();
     auto stopPersistence = high_resolution_clock::now();
    auto durationPersistence = duration_cast<microseconds>(stopPersistence - startPersistence);
    cout << "Index created from persistence in: " << durationPersistence.count()/double(1000000) << " seconds." << endl;
}

void Index_Handler::write_to_persistence(){
        write_word();
        write_org();
        write_person();
}

void Index_Handler::write_word(){
    ofstream persist;
        persist.open("./persistentWord.txt"); //may overwrite if done for multiple trees
        
        wordTree.level_order_traversal();
        const auto& wT = wordTree.get_level_ordered_nodes(); //pair<string,pair<vector<pair<string,int>>>>;
        cout << "Writing this to persistentOrg.txt \n";
        
        for (auto &wordObject : wT)
        {
            // print in a way you can read it
            persist << wordObject.first << ':';
            for (auto &documentObject : wordObject.second)
            {   
                persist << documentObject.first << ',' << documentObject.second << ';';
            }
            persist << endl;
            // uniqueOrgs++; //increment unique orgs by one to keep track of this statistic
        }
        persist.close();

}

void Index_Handler::write_org(){          //writes orgTree nodes to a file
    ofstream persist;
        persist.open("./persistentOrg.txt"); //may overwrite if done for multiple trees
        
        orgTree.level_order_traversal();
        const auto& wT = orgTree.get_level_ordered_nodes(); //pair<string,pair<vector<pair<string,int>>>>;

        cout << "Writing this to persistentOrg.txt \n";
        
        for (auto &orgObject : wT)
        {
            // print in a way you can read it
            persist << orgObject.first << ':';
            for (auto &documentObject : orgObject.second)
            {   
                persist << documentObject.first << ',' << documentObject.second << ';';
            }
            persist << endl;
            // uniqueOrgs++; //increment unique orgs by one to keep track of this statistic
        }
        persist.close();
    }

    void Index_Handler::write_person(){        //writes personTree nodes to a file
    ofstream persist;
        persist.open("./persistentPerson.txt"); //may overwrite if done for multiple trees
        
        personTree.level_order_traversal();
        const auto& wT = personTree.get_level_ordered_nodes(); //pair<string,pair<vector<pair<string,int>>>>;

        cout << "Writing this to persistentPerson.txt \n";
        
        for (auto &personObject : wT)
        {
            // print in a way you can read it
            persist << personObject.first << ':';
            for (auto &documentObject : personObject.second)
            {   
                persist << documentObject.first << ',' << documentObject.second << ';';
            }
            persist << endl;
            // uniquePersons++; //increment unique persons by one to keep track of this statistic
        }
        persist.close();

    }

    void Index_Handler::read_word(){ //reads from permenent tree file, and recreates an AVL tree
    uniqueWords=0;
    ifstream persist;
    persist.open("./persistentWord.txt"); //may overwrite if done for multiple trees
        if (persist.is_open()) {
            cout << "file open" << endl;
        }else{cout << "ERROR Persistence File could not be opened"; 
            return;
        }
    string wordNode;
    string word;
    string curDoc;
    string remDoc;
    string freq;
    size_t semiColon;
    size_t colon;
    size_t comma;
    size_t newLine=0;
    // size_t var;
    while(getline(persist, wordNode, '\n')){

        colon = wordNode.find(":");
        newLine = wordNode.length()-1;
        word = wordNode.substr(0,colon); //word is just the word
        remDoc = wordNode.substr(colon+1, newLine-colon); //doc starts as the whole line
        comma = remDoc.find(',');
        colon = remDoc.find(":");
        curDoc = remDoc.substr(colon+1,comma-colon-1); //(colon+1,comma-colon);
        freq = remDoc.substr(comma+1, 1);
        string k = word;
        vector<pair<string,int>> v;
        pair<string, int> pair1(curDoc, stoi(freq)); //stoi converts freq from string to int 
        v.push_back(pair1);
        semiColon = remDoc.find(';');
        newLine = remDoc.length()-1;
        if(newLine-semiColon == 0){ //for when the word appears in just one document
             pair<string, vector<pair<string, int>>> data1(k,v);
            wordTree.insert(data1);
            uniqueWords++;
            continue;
        }
        remDoc = remDoc.substr(semiColon, newLine-semiColon+1);
        while(remDoc.find(';')!=string::npos){
            newLine = remDoc.length()-1;
            comma = remDoc.find(',');
            semiColon = remDoc.find(";");
            curDoc = remDoc.substr(0,comma);
            freq = remDoc.substr(comma+1, 1);
            pair<string, int> pair2(curDoc, stoi(freq)); //stoi converts freq from string to int 
            v.push_back(pair2); //check this out
            semiColon = remDoc.find(';',+2);
            newLine = remDoc.length()-1;
            if(semiColon == string::npos){
                pair<string, vector<pair<string, int>>> data1(k,v);
                wordTree.insert(data1);
                break;
            }
            remDoc = remDoc.substr(semiColon+1, newLine-semiColon+1); //doc starts as the whole line
            semiColon = remDoc.find(';');
            newLine = remDoc.length()-1;
            if(newLine==semiColon){
                continue;
            }
        }
    pair<string, vector<pair<string, int>>> data1(k,v);
    wordTree.insert(data1);
    }
    persist.close();       //closes permenentTree.txt
}
    void Index_Handler::read_org(){                //create orgTree from text file
    uniqueOrgs=0;
     ifstream persist;
    persist.open("./persistentOrg.txt"); //may overwrite if done for multiple trees
        if (persist.is_open()) {
            cout << "file open" << endl;
        }else{cout << "ERROR Persistence (org) File could not be opened"; 
            return;
        }
    string wordNode;
    string word;
    string curDoc;
    string remDoc;
    string freq;
    size_t semiColon;
    size_t colon;
    size_t comma;
    size_t newLine=0;
    // size_t var;
    while(getline(persist, wordNode, '\n')){
        colon = wordNode.find(":");
        newLine = wordNode.length()-1;
        word = wordNode.substr(0,colon); //word is just the word
        remDoc = wordNode.substr(colon+1, newLine-colon); //doc starts as the whole line
        comma = remDoc.find(',');
        colon = remDoc.find(":");
        curDoc = remDoc.substr(colon+1,comma-colon-1); //(colon+1,comma-colon);
        freq = remDoc.substr(comma+1, 1);
        string k = word;
        vector<pair<string,int>> v;
        pair<string, int> pair1(curDoc, stoi(freq)); //stoi converts freq from string to int 
        v.push_back(pair1);
        semiColon = remDoc.find(';');
        newLine = remDoc.length()-1;
        if(newLine-semiColon == 0){ //for when the word appears in just one document
             pair<string, vector<pair<string, int>>> data1(k,v);
            orgTree.insert(data1);
            uniqueOrgs++;
            continue;
        }
        remDoc = remDoc.substr(semiColon, newLine-semiColon+1);
        while(remDoc.find(';')!=string::npos){
            newLine = remDoc.length()-1;
            comma = remDoc.find(',');
            semiColon = remDoc.find(";");
            curDoc = remDoc.substr(0,comma);
            freq = remDoc.substr(comma+1, 1);
            pair<string, int> pair2(curDoc, stoi(freq)); //stoi converts freq from string to int 
            v.push_back(pair2);
            semiColon = remDoc.find(';',+2);
            newLine = remDoc.length()-1;
            if(semiColon == string::npos){
                pair<string, vector<pair<string, int>>> data1(k,v);
                orgTree.insert(data1);
                break;
            }
            remDoc = remDoc.substr(semiColon+1, newLine-semiColon+1); //doc starts as the whole line
            semiColon = remDoc.find(';');
            newLine = remDoc.length()-1;
            if(newLine==semiColon){
                continue;
            }
        }
    pair<string, vector<pair<string, int>>> data1(k,v);
    orgTree.insert(data1);
    }
    persist.close();       //closes permenentTree.txt

    }
    
    void Index_Handler::read_person(){             //create personTree from text file
    uniquePersons=0;
     ifstream persist;
    persist.open("./persistentPerson.txt"); //may overwrite if done for multiple trees
        if (persist.is_open()) {
            cout << "file open" << endl;
        }else{cout << "ERROR Persistence File could not be opened (person)"; 
            return;
        }
    string wordNode;
    string word;
    string curDoc;
    string remDoc;
    string freq;
    size_t semiColon;
    size_t colon;
    size_t comma;
    size_t newLine=0;
    // size_t var;
    while(getline(persist, wordNode, '\n')){
        colon = wordNode.find(":");
        newLine = wordNode.length()-1;
        word = wordNode.substr(0,colon); //word is just the word
        remDoc = wordNode.substr(colon+1, newLine-colon); //doc starts as the whole line
        comma = remDoc.find(',');
        colon = remDoc.find(":");
        curDoc = remDoc.substr(colon+1,comma-colon-1); //(colon+1,comma-colon);
        freq = remDoc.substr(comma+1, 1);
        string k = word;
        vector<pair<string,int>> v;
        pair<string, int> pair1(curDoc, stoi(freq)); //stoi converts freq from string to int 
        v.push_back(pair1);
        semiColon = remDoc.find(';');
        newLine = remDoc.length()-1;
        if(newLine-semiColon == 0){ //for when the word appears in just one document
             pair<string, vector<pair<string, int>>> data1(k,v);
            personTree.insert(data1);
            uniquePersons++;
            continue;
        }
        remDoc = remDoc.substr(semiColon, newLine-semiColon+1);
        while(remDoc.find(';')!=string::npos){
            newLine = remDoc.length()-1;
            comma = remDoc.find(',');
            semiColon = remDoc.find(";");
            curDoc = remDoc.substr(0,comma);
            freq = remDoc.substr(comma+1, 1);
            pair<string, int> pair2(curDoc, stoi(freq)); //stoi converts freq from string to int 
            v.push_back(pair2);
            semiColon = remDoc.find(';',+2);
            newLine = remDoc.length()-1;
            if(semiColon == string::npos){
                pair<string, vector<pair<string, int>>> data1(k,v);
                personTree.insert(data1);
                break;
            }
            remDoc = remDoc.substr(semiColon+1, newLine-semiColon+1); //doc starts as the whole line
            semiColon = remDoc.find(';');
            newLine = remDoc.length()-1;
            if(newLine==semiColon){
                continue;
            }
        }
    pair<string, vector<pair<string, int>>> data1(k,v);
    personTree.insert(data1);
    }
    persist.close();       //closes permenentTree.txt
    }

const vector<pair<string,int>> Index_Handler::get_wordData(string key){ // returns the vector of pairs of document file paths and frequency for the word given as a parameter
    if(wordTree.search(key)){ //if word is in tree
         auto data = wordTree.find(key);
         vector<pair<string,int>> wordData = data->second;
        return wordData;
    } 
    else{ // cout error message if word is not found
        cout << "ERROR! word: " << key << ", not found in wordTree" << endl;
        vector<pair<string,int>> empty_vec;
        return empty_vec;
    }
}

const vector<pair<string,int>> Index_Handler::get_orgData(string key){ // returns the vector of pairs of document file paths and frequency for the org given as a parameter
    if(orgTree.search(key)){ //if org is in tree
         auto data = orgTree.find(key); 
         vector<pair<string,int>> orgData = data->second;
        return orgData;
    } 
    else{ // cout error message if org is not found
        cout << "ERROR! word: " << key << ", not found in orgTree" << endl;
        vector<pair<string,int>> empty_vec;
        return empty_vec;
    }
}

const vector<pair<string,int>> Index_Handler::get_personData(string key){ // returns the vector of pairs of document file paths and frequency for the person given as a parameter
    if(personTree.search(key)){ //if person is in tree
         auto data = personTree.find(key);
         vector<pair<string,int>> personData = data->second;
        return personData;
    } 
    else{ // cout error message if person is not found
        cout << "ERROR! word: " << key << ", not found in personTree" << endl;
        vector<pair<string,int>> empty_vec;
        return empty_vec;
    }
}

//return other statistics
const int Index_Handler::get_uniqueWords(){
    return uniqueWords;
}
const int Index_Handler::get_uniqueOrgs(){
    return uniqueOrgs;
}
const int Index_Handler::get_uniquePersons(){
    return uniquePersons;
}

void Index_Handler::print_Tree_in_order(){
    wordTree.in_order_traversal();
}
       
void Index_Handler::clean_allTrees(){ //empties all avl trees in Index_Handler
    clean_wordTree();
    clean_orgTree();
    clean_personTree();
}
 
void Index_Handler::clean_wordTree(){
    wordTree.clean();
}

void Index_Handler::clean_orgTree(){
    orgTree.clean();
}

void Index_Handler::clean_personTree(){
    personTree.clean();
}

