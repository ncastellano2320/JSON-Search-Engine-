//
// Created by ncast on 4/2/2023.
//
#include "Document_Parser.h"

Document_Parser::Document_Parser() {
    //Populate stopwords set
    std::ifstream file("../stopwords.txt");  // Open the text file for reading
    if (!file.is_open()) {  // Check if the file was successfully opened
        std::cerr << "Failed to open the file!" << std::endl;
    }

    std::string line;
    while (std::getline(file, line)) {  // Read each line from the file
        stop_words.insert(line);  // Add the line to the vector
    }

    file.close();  // Close the stop_words file after reading
}

void Document_Parser::clean_text(string &text_to_clean) {
    //Remove \n
    text_to_clean = regex_replace(text_to_clean, regex("\n"), "");
    //1. Tokenize
    char* token;
    char* raw_text = new char[text_to_clean.length() + 1];
    strcpy(raw_text, text_to_clean.c_str());
    char delim[] = ".~""',-!@#$%^&*()_+~`={}[]1234567890;:/||? ";
    token = strtok(raw_text, delim);
    while (token != nullptr) {
        string word = token;
        //Check if stopword
        if (stop_words.find(word) == stop_words.end()) {
            //Stemming (makes lowercase)
            Porter2Stemmer::trim(word);
            Porter2Stemmer::stem(word);
            //add text to vector of words if not 1-letter word or not a stopword
            if (word.length() > 1) {
                text.push_back(word);
            }
        }
        token = strtok(nullptr, delim);
    }
    delete[] raw_text;


}

void Document_Parser::ReadFileSystem(const string &path) {
    //Set Timer
    auto start = chrono::high_resolution_clock::now();

    // recursive_director_iterator used to "access" folder at parameter -path-
    // we are using the recursive iterator, so it will go into sub-folders.
    // see: https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
    auto it = filesystem::recursive_directory_iterator(path);
    index_handler.resetStats();
    // loop over all the entries.
    for (const auto &entry : it)
    {
        // We only want to attempt to parse files that end with .json...
        if (entry.is_regular_file() && entry.path().extension().string() == ".json")
        {
            ReadJsonFile(entry.path().string());
        }
    }

    cout << "Documents Parsed Succesfully!" << endl;
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << "seconds\n";
}

void Document_Parser::ReadJsonFile(const string &fileName) {
    // open an ifstream on the file of interest and check that it could be opened.
    ifstream input(fileName);
    if (!input.is_open())
    {
        cerr << "cannot open file: " << fileName << endl;
        return;
    }

    // Create a RapidJSON IStreamWrapper using the file input stream above.
    IStreamWrapper isw(input);

    // Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
    Document d;
    d.ParseStream(isw);

    // Now that the document is parsed, we can access different elements the JSON using
    // familiar subscript notation.

    //Get Text from Json
    auto doc_text = d["text"].GetString();
    string t = doc_text;
    //Clean text and add to vector
    clean_text(t);

    //Get Organizations from Json
    auto organizations = d["entities"]["organizations"].GetArray();
    for (auto &p : organizations)
    {
        string the_org = p["name"].GetString();
        // Convert the string to lowercase
        for (char& c : the_org) {
            c = std::tolower(c);
        }
        char* token;
        char* raw_text = new char[the_org.length() + 1];
        strcpy(raw_text, the_org.c_str());
        char delim[] = ".- ";
        token = strtok(raw_text, delim);
        while (token != nullptr) {
            string orgi = token;
            org.push_back(orgi);
            token = strtok(nullptr, delim);
        }
        delete[] raw_text;
    }

    //Get Persons from Json
    auto persons = d["entities"]["persons"].GetArray();
    for (auto &p : persons)
    {
        string name = p["name"].GetString();
        // Convert the string to lowercase
        for (char& c : name) {
            c = std::tolower(c);
        }
        char* token;
        char* raw_text = new char[name.length() + 1];
        strcpy(raw_text, name.c_str());
        char delim[] = ".- ";
        token = strtok(raw_text, delim);
        while (token != nullptr) {
            string the_name = token;
            people.push_back(the_name);
            token = strtok(nullptr, delim);
        }
        delete[] raw_text;
    }

    //add to text map
    // iterate over the elements in the vector
    for (const auto& element : text) {
        // check if the element is present in the map
        auto mapIter = text_map.find(element);
        if (mapIter != text_map.end()) {
            //Is present so increment frequency
            mapIter->second.second ++;
        } else {
            //Is not present so add to map
            text_map.insert(make_pair(element, make_pair(fileName,1)));
        }
    }

    //add to org map
    // iterate over the elements in the vector
    for (const auto& element : org) {
        // check if the element is present in the map
        auto mapIter = org_map.find(element);
        if (mapIter != org_map.end()) {
            //Is present so increment frequency
            mapIter->second.second ++;
        } else {
            //Is not present so add to map
            org_map.insert(make_pair(element, make_pair(fileName,1)));
        }
    }

    //add to people map
    for (const auto& element : people) {
        // check if the element is present in the map
        auto mapIter = person_map.find(element);
        if (mapIter != person_map.end()) {
            //Is present so increment frequency
            mapIter->second.second ++;
        } else {
            //Is not present so add to map
            person_map.insert(make_pair(element, make_pair(fileName,1)));
        }
    }

    //Add to trees or add to hash-maps
    index_handler.add_to_textTree(text_map);
    index_handler.add_to_orgTree(org_map);
    index_handler.add_to_personTree(person_map);

    //Clear Vectors
    text.clear(), org.clear(), people.clear();
    //Clear Maps
    text_map.clear(), org_map.clear(), person_map.clear();

    input.close();

}

void Document_Parser::write_to_Persistence() { //writes word tree nodes to file
    index_handler.write_to_persistence();
}

void Document_Parser::read_From_Persistence() { //writes word tree nodes to file
    index_handler.read_from_persistence();
}

void Document_Parser::clear_index() {
    index_handler.clean_allTrees();
}

void Document_Parser::print_wordTree_in_order(){
    index_handler.print_Tree_in_order();
}

Index_Handler Document_Parser::get_Index_Handler(){ //returns index handler
    return index_handler; 
}

void Document_Parser::preview_file_data(const string &file_path) {
    // open an ifstream on the file of interest and check that it could be opened.
    ifstream input(file_path);
    if (!input.is_open())
    {
        cerr << "cannot open file: " << file_path << endl;
        return;
    }

    // Create a RapidJSON IStreamWrapper using the file input stream above.
    IStreamWrapper isw(input);

    // Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
    Document d;
    d.ParseStream(isw);

    // Now that the document is parsed, we can access different elements the JSON using
    // familiar subscript notation.

    //Get Title
    auto title = d["title"].GetString();

    //Get publication/authors
    auto authors = d["author"].GetString();

    //Get Date Published 
    auto publication = d["published"].GetString();

    //Get url
    auto url = d["url"].GetString();

    cout << title << endl << url << endl << "Publisher: " << authors << endl << "Date Published: " << publication << endl;

    input.close();

}

void Document_Parser::single_file_info(const string &file_path) {

    // open an ifstream on the file of interest and check that it could be opened.
    ifstream input(file_path);
    if (!input.is_open())
    {
        cerr << "cannot open file: " << file_path << endl;
        return;
    }

    // Create a RapidJSON IStreamWrapper using the file input stream above.
    IStreamWrapper isw(input);

    // Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
    Document d;
    d.ParseStream(isw);

    // Now that the document is parsed, we can access different elements the JSON using
    // familiar subscript notation.

    //Get Title
    auto title = d["title"].GetString();

    //Get publication/authors
    auto authors = d["author"].GetString();

    //Get text
    auto doc_text = d["text"].GetString();

    cout << endl << title << endl << authors << endl << doc_text << endl << endl;

    input.close();

}



