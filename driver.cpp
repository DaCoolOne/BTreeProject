

#include <iostream>
#include <cstdlib>

#include "BinaryTree.h"
#include "AvlTree.h"
#include "BTree.h"
#include "BPlusTree.h"

using b_tree::Tree;

#define TREE_TYPE b_tree::BPlusTree

using std::cout;
using std::endl;
using std::rand;

void test(Tree<int>* tree)
{
    const static int num_keys = 40;
    int* keys = new int[num_keys];

    int temp;

    cout << "Insertion of items" << endl;
    int a, b;
    int i = 0;
    while(i < num_keys)
    {
        a = rand() % 100;
        b = rand() % 100;
        // cout << a << "," << b << endl;

        cout << "\"Insert: " << a << "\"," << endl;
        // Output I can directly copy+paste into the javascript program that I wrote for visualizing the tree.
        if (tree->insert(a, new int(b))) {
            //cout << " SUCCESS!\"," << endl;
            keys[i] = a;
            i++;
        }
        else {
            //cout << " FAILURE!\"," << endl;
        }
        cout << '"';
        tree->show(cout);
        cout << "\"," << endl;
        
    }

    cout << "Node count: " << tree->count() << endl;
    cout << "Depth: " << tree->depth() << endl;

    cout << "Diagram:" << endl;
    tree->show(cout);
    cout << endl << endl;

    cout << "Access elements:" << endl;
    for(int i = 0; i < 10; i ++) {
        int key = keys[rand() % num_keys];
        int* val = tree->get(key);
        if(val) {
            cout << key << ": " << (*val) << endl;
        } else {
            cout << key << ": nullptr" << endl; // This should never happen in testing.
        }
    }

    cout << "Search tree:" << endl;
    for(int i = 0; i < 10; i ++) {
        cout << "Searching for " << keys[i] << endl;
        const int* val = tree->get(keys[i]);
        int result;
        bool found = tree->search(result, val);
        if(found)
            cout << *val << ": " << result << endl;
        else
            cout << *val << ": not found" << endl;
    }

    cout << "Random search tree" << endl;
    for(int i = 0; i < 10; i ++) {
        const int val = rand() % 100;
        int result;
        bool found = tree->search(result, &val);
        if(found)
            cout << val << ": " << result << endl;
        else
            cout << val << ": not found" << endl;
    }

    cout << "Access random keys" << endl;
    for(int i = 0; i < 10; i ++) {
        int key = rand() % 100;
        int* val = tree->get(key);
        if(val)
            cout << key << ": " << *val << endl;
        else
            cout << key << ": nullptr" << endl;
    }

    cout << "Remove nodes from tree, insert new random nodes" << endl;
    for(int i = 0; i < num_keys; i ++) {
        cout << "Removing key " << keys[i] << endl;
        tree->remove(keys[i]);
        
        cout << "Diagram:" << endl;
        tree->show(cout);

        int k = rand() % 100;
        cout << "New key " << k << endl;
        tree->insert(k, new int(rand() % 100));

        cout << "Diagram:" << endl;
        tree->show(cout);
    }
    
    cout << "Diagram:" << endl;
    tree->show(cout);

    /**/

    cout << "Node count: " << tree->count() << endl;
    cout << "Depth: " << tree->depth() << endl;

    delete[] keys;
}

int main()
{
    srand(16);

    cout << "Building tree" << endl;

    Tree<int>* tree = new TREE_TYPE<int>();

    cout << "Start testing" << endl;

    test(tree);
    
    cout << "Test end, deleting" << endl;

    delete tree;

    cout << "Run complete." << endl;

    return 0;
}
