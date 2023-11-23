//AVLTree.h file
// Created by ncast on 4/2/2023.
//
#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <queue> // for level-order traversal
#include <utility>

using namespace std;

// AvlTreeMap class

template <typename K,typename V>
class AVLTree {
private:
    struct AvlNode
    {
        pair<K,V> data;
        AvlNode *left;
        AvlNode *right;
        int height;

        AvlNode(const K &key, const V &value, AvlNode *lt, AvlNode *rt, int h = 0)
                : data{key, value}, left{lt}, right{rt}, height{h} {}
    };

    AvlNode *root;
    queue<AvlNode*> levelQueue;
    // vector<string> levelOrderedNodes;
    vector<pair<K,V>> levelOrderedNodes;
    // vector<string> levelOrderedNodeStrings;


    //Helper Functions
    //Helper Function for copy constructor and copy assignment
    //Clone
    AvlNode* clone(AvlNode *t) const
    {
        if (t == nullptr)
            return nullptr;

        return new AvlNode{t->data.first, t->data.second, clone(t->left), clone(t->right), t->height};
    }
    //insert
    void insert(const pair<K,V> &x, AvlNode *&t) {
        if (t == nullptr)
        {
            t = new AvlNode{x.first, x.second, nullptr, nullptr, 0};
            return; // a single node is always balanced
        }

        if (x.first < t->data.first)
            insert(x, t->left);
        else if (t->data.first < x.first)
            insert(x, t->right);
        else
        {
        } // Duplicate; do nothing

        // This will call balance on the way back up the tree. It will only balance
        // once at node the where the tree got imbalanced (called node alpha in the textbook)
        // and update the height all the way back up the tree.
        balance(t);
    }
    //make_empty
    void makeEmpty(AvlNode *&t)
    {
        if (t == nullptr)
            return;

        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;  // deletes the node at t, 
        t = nullptr;
    }
    //in_order-traversal
    void in_order_traversal(AvlNode *t, ostream &out) const
    {
        if (t == nullptr)
            return;
        
        // recursion
        printTreeSort(t->left, out);
        out << t->data.first << " ";
        if(t==root){
        out << t->data.first << " ";
        }
        printTreeSort(t->right, out);
    }
    //level_order_traversal
    void level_order_traversal(AvlNode *t, ostream &out)  //replace cout with file out for persistence
    {

        if (t == nullptr) //base case, stop when reached
            return;
        if(t->left == nullptr){ //if left is nullptr, right is pushed to queue first
            if(t->right != nullptr){levelQueue.push(t->right);} //if not a nullptr, add right node to queue
            if(t->left != nullptr){levelQueue.push(t->left);} //if not nullptr, add left node to queue
             }

            else{//otherwise, left is pushed first
        if(t->left != nullptr){levelQueue.push(t->left);} //if not nullptr, add left node to queue
        if(t->right != nullptr){levelQueue.push(t->right);}//if not a nullptr, add right node to queue
        }

        levelOrderedNodes.push_back(t->data);
        levelQueue.pop();
        if(levelQueue.empty()==true){return;} // this line messes up. is not stopping.
        level_order_traversal(levelQueue.front(), out);
    }

   
     /**
     * Internal recursive method to print a subtree rooted at t in sorted order.
     * This is inorder traversal (LNR)
     */
    void printTreeSort(AvlNode *t, ostream &out) const
    {
        if (t == nullptr)
            return;

        // recursion
        printTreeSort(t->left, out);
        //out << t->data.first << " ";
        out<< t->data.first << ':';
        
        out << endl;
        printTreeSort(t->right, out);
    }

    void preOrderTreeSort(AvlNode *t, ostream &out) const
    {
        if (t == nullptr)
            return;

        //out << t->data.first << " ";
        out<< t->data.first << ':';
        for (auto &documentObject : t->data.second)
        {
            out << documentObject.first << ',' << documentObject.second << ';';
        }
        out << endl;
        // recursion
        preOrderTreeSort(t->left, out);
        preOrderTreeSort(t->right, out);
    }


    //max
    int max(int lhs, int rhs) const
    {
        return lhs > rhs ? lhs : rhs;
    }
    //height
    int height(AvlNode *t) const
    {
        return t == nullptr ? -1 : t->height;
    }
    //balance
    static const int ALLOWED_IMBALANCE = 1;
    void balance(AvlNode *&t)
    {
        if (t == nullptr)
            return;

        if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) // unbalancing insertion was left
        {
            if (height(t->left->left) >= height(t->left->right))
                rotateWithLeftChild(t); // case 1 (outside)
            else
                doubleWithLeftChild(t); // case 2 (inside)
        }
        else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) // unbalancing insertion was right
        {
            if (height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t); // case 4 (outside)
            else
                doubleWithRightChild(t); // case 3 (inside)
        }

        // update height
        t->height = max(height(t->left), height(t->right)) + 1;
    }
    //rotate-with-left-Child
    void rotateWithLeftChild(AvlNode *&k2)
    {
        AvlNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;
        k2 = k1;
    }
    //rotate-with-right-child
    void rotateWithRightChild(AvlNode *&k1)
    {
        AvlNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->right), k1->height) + 1;
        k1 = k2;
    }
    //double-with-left-child
    void doubleWithLeftChild(AvlNode *&k3)
    {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }
    //double-with-right-child
    void doubleWithRightChild(AvlNode *&k1)
    {
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }
    //Helper Function for search
    bool Search_Helper(AvlNode* node, K key) {
        if (node == nullptr) {
            return false;
        } else if (key < node->data.first) {
            return Search_Helper(node->left, key);
        } else if (key > node->data.first) {
            return Search_Helper(node->right, key);
        } else {
            return true;
        }
    }
public:

    //Constructor
    AVLTree() {
        root = nullptr;
    }
    //Copy Constructor
    AVLTree(const AVLTree &rhs) : root{nullptr}
    {
        root = clone(rhs.root);
    }
    //Copy Assignment Operator
    AVLTree &operator=(const AVLTree &rhs)
    {
        makeEmpty(root);
        root = clone(rhs.root);
        return *this;
    }
    //Destructor
    ~AVLTree()
    {
        makeEmpty(root);
    }
    //void insert function
    void insert(const pair<K,V> &x)
    {
        insert(x, root);
    }
    //is empty function
    bool isEmpty() const {
        return root == nullptr;
    }
    //get_height
    int height_ofRoot() {
        return root == nullptr ? -1 : root->height;
    }
    //in_order_traversal()
    void in_order_traversal(ostream &out = cout) const
    {
        in_order_traversal(root, out);
    }

    void pre_order_traversal(ostream& out){
        preOrderTreeSort(root, out);
    }
    //level_order_traversal
    void level_order_traversal(ostream &out = cout)  //replace cout with file out to persistence
    {
        levelOrderedNodes.clear(); //clears the levelOrderedNodes vector
        levelQueue.push(root);      // pushes the root to the queue
        level_order_traversal(root, out);   //goes to helper function
    }
    //Object find() - returns the data from the key
    pair<K,V>* find(K keyItem){     
        AvlNode* temp = root;
        while(1==1){
            if(temp == nullptr){
                return nullptr;
            }

            if (temp->data.first < keyItem){
                temp = temp->right;
            } else if(temp->data.first > keyItem){
                temp = temp->left;
            } else{
                return &temp->data;
            }
        }
    }
    void clean()
    {
        makeEmpty(root);
    }
    //Search Function - returns true if the key is found
    bool search(K key) {
        return Search_Helper(root, key);
    }


    const vector<pair<K,V>>& get_level_ordered_nodes(){
         return levelOrderedNodes;
    }
};

#endif