#define CATCH_CONFIG_MAIN
//
// Created by ncast on 3/30/2023.
//
// @@ -7,12 +8,44 @@
#include "Index_Handler.h"
#include "AVLTree.h"
#include <iostream>
#include <string>
#include "catch2/catch.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

TEST_CASE("AVL Tree Insertions and sorting", "[AVL_Tree]")
{
    AVLTree<int, int> intTree;
    pair<int, int> hello(10, 100);
    intTree.insert(hello);
    auto hello2 = intTree.find(hello.first);
    REQUIRE(hello2->second == 100);
    //
    REQUIRE(hello2->first == 10);
    //
    // SECTION("many ints test")
    // {
        AVLTree<int, int> randIntTree;
        // Seed the random number generator
        srand(time(nullptr));
        // Create a vector of randomized integers
        vector<int> k;
        vector<int> v;
        for (int i = 0; i < 100; ++i)
        {
            k.push_back(i);
            v.push_back(rand() % 10000); // Generate random integer between 0-99
        }

        // Iterate through the vector and print its elements
        for (int i = 0; i < v.size(); ++i)
        {
            pair<int, int> tempPair = pair(k[i], v[i]);
            randIntTree.insert(tempPair);
        }
        cout << endl;
        randIntTree.in_order_traversal(cout); 
        cout << endl;
        // cout << randIntTree.find(63)->first << randIntTree.find(63)->second << endl;
        REQUIRE(v[63]==randIntTree.find(63)->second);
        REQUIRE(v[0]==randIntTree.find(0)->second);
        REQUIRE(v[99]==randIntTree.find(99)->second);
        REQUIRE(randIntTree.height_ofRoot()==6);
      
        REQUIRE(randIntTree.search(30)==true);
        AVLTree<int,int> copyIntTree = randIntTree;
        REQUIRE(randIntTree.isEmpty()==false);
        randIntTree.clean();
        REQUIRE(randIntTree.isEmpty()==true);
        REQUIRE(copyIntTree.isEmpty()==false);
        REQUIRE(randIntTree.search(30)==false);
        copyIntTree.level_order_traversal();
    // }
   
// }

// // Testing AVL Tree Constructor and Is empty-------------------------------------------------------------------------------------------------

// // //     bool empty = tree.isEmpty();
// // //     if (empty == 1) {
// // //         cout << "true" << endl;
// // //     } else if (empty == 0) {
// // //         cout << "false" << endl;
// // //     }

// //     //Testing AVL Tree Copy Constructor--------------------------------------------------------------------------------------------
// //     //Testing AVL Tree Copy Assignment Operator AVLTree(const AVLTree& other)------------------------------------------------------
// //     //Testing AVL Tree Copy Assignment Operator AVLTree& operator=(const AVLTree& other)-------------------------------------------
AVLTree<int, int> tree;
AVLTree<int, int> copy = tree;
// Testing void insert(object data) Function------------------------------------------------------------------------------------
tree.insert(hello);
pair<int, int> *copypair = tree.find(hello.first);
pair<int, int> goodbye(6, 60);
tree.insert(goodbye);
pair<int, int> bb(3, 30);
tree.insert(bb);
pair<int, int> bbc(7, 70);
tree.insert(bbc);
pair<int, int> dd(12, 120);
tree.insert(dd);
pair<int, int> zz(17, 170);
tree.insert(zz);
pair<int, int> yy(14, 140);
tree.insert(yy);
tree.level_order_traversal();
tree.in_order_traversal();
    int x = 12;

AVLTree<string, vector<pair<string,vector<string>>>> tt; // proper format for tree?

bool twelve = tree.search(x);
REQUIRE(twelve==true);
// Testing void remove(object data) function------------------------------------------------------------------------------------

// Testing bool search(object data) const function------------------------------------------------------------------------------
vector<pair<string, int>> vector1;
pair<string, int> pair1("ninodoc", 4);
vector1.push_back(pair1);
pair<string, vector<pair<string, int>>> data("nino", vector1);
AVLTree<string, vector<pair<string, int>>> wordTree1;
wordTree1.insert(data);
vector<pair<string, int>> vector2;
pair<string, int> pair2("noahdoc", 2);
vector2.push_back(pair2);
vector<pair<string, int>> vector3;
 pair<string, int> pair3("guestdoc", 6);
vector3.push_back(pair3);
pair<string, vector<pair<string, int>>> data2("noah", vector2);
pair<string, vector<pair<string, int>>> data3("guest", vector3);
wordTree1.insert(data2);
wordTree1.insert(data3);
unordered_map<string, pair<string, int>> mapdata;
mapdata["nino"] = pair1;
mapdata["noah"] = pair2;
mapdata["adam"] = pair2;
mapdata["blue"] = pair2;
mapdata["red"] = pair2;
mapdata["green"] = pair2;
mapdata["jack"] = pair2;
mapdata["juice"] = pair2;
mapdata["yellow"] = pair2;
mapdata["purple"] = pair2;
mapdata["color"] = pair2;
Index_Handler hand;
hand.add_to_textTree(mapdata);
mapdata.clear();
mapdata["color"] = pair1;
mapdata["juice"] = pair1;
hand.add_to_textTree(mapdata);
mapdata.clear();
  mapdata["color"] = pair3;
  hand.add_to_textTree(mapdata);
hand.write_to_persistence();
hand.read_from_persistence();
//testing get_wordData();
vector<pair<string,int>> colorData = hand.get_wordData("color"); //should return the Value from data of "color", which is a vector of pairs
REQUIRE(string(colorData[0].first) == "noahdoc"); //should cout noahdoc
REQUIRE(colorData[0].second == 2); //should cout 2
REQUIRE(string(colorData[1].first) == "ninodoc"); //should cout ninodoc
REQUIRE(colorData[1].second == 4); //should cout 4
}
/** Just Some Notes on helper functions
 * Note they can be called in the struct class or AVL tree class if struct is built into AVLTree class.
 * Helper functions are functions that are not directly called by the user,
 * but are used by other functions in the class to help perform their tasks.
 * In the case of an AVL tree, helper functions are typically used to recursively
 * traverse the tree or to perform specific operations on nodes. Some examples of
 * helper functions that may be used in an AVL tree include:

- copyTree: This function is used by the copy constructor and copy assignment operator to make a deep copy of the tree.
- destroyTree: This function is used by the destructor to recursively delete all nodes in the tree.
- rotateRight and rotateLeft: These functions are used to perform right and left rotations, respectively, on nodes in the tree to balance it.
- balance: This function is used to balance a node and its children by checking their heights and performing rotations if necessary.
- getHeight: This function is used to recursively calculate the height of a given node.
- inOrderTraversal, preOrderTraversal, and postOrderTraversal: These functions are used to recursively traverse the tree in a specific order and perform an operation on each node.
- getMinValue and getMaxValue: These functions are used to recursively find the minimum and maximum values in the tree.
- findNode, findSuccessor, and findPredecessor: These functions are used to recursively search the tree for a given node or its successor/predecessor.
- isBalanced: This function is used to recursively check if the tree is balanced.
- getSize: This function is used to recursively calculate the number of nodes in the tree.
 */
// Just Some Notes on helper functions