//
// Created by ncast on 4/2/2023.
//
#include "User_Interface.h"

User_Interface::User_Interface() = default;

void User_Interface::start() {
    
    cout << "Welcome to our Search Engine" << endl;
    cout << "Here are your options: " << endl;
    cout << "-------------------------------------" << endl;
    cout << "(1) Run Document Parser on File System " << endl;  
    cout << "(2) Create Persistence File " << endl;
    cout << "(3) Read Persistence File " << endl;
    cout << "(4) Clear Index " << endl;
    cout << "(5) Search a Query " << endl;
    cout << "(6) Output Statistics " << endl;
    cout << "(7) Exit " << endl;
    int answer;
    cout << "Please Choose One: ";
    cin >> answer;

    
    if (answer == 1) {

        runDocParser();

    } else if (answer == 2) {

        createPersistance();

    } else if (answer == 3) {

        readPersistance();

    } else if (answer == 4) {

        clearIndex();

    } else if (answer == 5) {

        query_search();

    } else if (answer == 6) {

        stats();

    } else if (answer == 7) {

        cout << "Have a nice day :)" << endl;
        return;

    } else if (answer > 7 || answer<1) {

        cout << "Invalid Choice. Please Try Again" << endl;
    }

    
    //Recursive call to search Engine Menu
    start();


}

void User_Interface::runDocParser() {

    string directory;
    cout << "Please Enter the Directory You Want to Parse: ";
    cin >> directory;
    documentParser.ReadFileSystem(directory);
    cout << endl;

}

void User_Interface::createPersistance() {

    documentParser.write_to_Persistence();

}

void User_Interface::readPersistance() {

    documentParser.read_From_Persistence();

}

void User_Interface::clearIndex() {
    documentParser.clear_index(); //clear the trees from the index
    cout << "INDEXES CLEARED" << endl;
}

void User_Interface::query_search() {
    string input;
    cin.ignore();
    cout << "Search: ";
    getline(cin,input);
    auto index = documentParser.get_Index_Handler();
    auto start = high_resolution_clock::now(); //start chrono timer to track runtime
    Query_Processor query(input, index);
    vector<pair<string,int>> results = query.return_final_intersection();
    vector<pair<string,int>> top_15;


    if (!results.empty()) {
        int i = 0;
        while (results.size() - i > 0 && i < 15) {
            top_15.push_back(results[results.size() - i - 1]);
            i++;
        }

        int paginate = 1;
        for (const auto &index: top_15) {
            cout << paginate << ". ";
            documentParser.preview_file_data(index.first);
            cout << endl;
            paginate++;
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Query found: " << i << " matches in " << duration.count()/double(1000000) << " seconds." << endl;

        cout << "Would you like to open any of these Documents? Enter the number of which one you'd like, if not enter 0 to exit: ";
        int answer;
        cin >> answer;

        if (answer == 0) {

            return;

        } else if (answer > 0 && answer <= 15) {

            documentParser.single_file_info(top_15[answer - 1].first);

        }
    }
}

void User_Interface::stats() {

    //show stats
    //Timing for indexing and for queries (use std::chrono)
    //Total number of individual articles in the current index
    //The total number of unique words indexed (total nodes in the word AVL Tree)
    //documentParser.get_Index_Handler().write_to_persistence();
    cout << "Total number of unique... " << endl; 
    cout << "Words: " << documentParser.get_Index_Handler().get_uniqueWords() << endl;
    cout << "Orgs: " << documentParser.get_Index_Handler().get_uniqueOrgs() << endl;
    cout << "Persons: " << documentParser.get_Index_Handler().get_uniquePersons() << endl;
    //Any other interesting stats that you gather in the course of parsing.

}

