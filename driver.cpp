

#include <iostream>
#include <cstdlib>
#include "BinaryTree.h"
#include "AvlTree.h"
using b_tree::Tree;
using b_tree::AvlTree;
using b_tree::BinaryTree;

using std::cout;
using std::endl;
using std::rand;

void test(Tree<int>* tree)
{
    const static int keys[] = { 30,1,12,300,23,70,40 };
    const static int num_keys = 7;

    int temp;

    cout << "Insertion of items" << endl;
    int a, b;
    for(int i = 0; i < 40; i ++)
    {
        a = rand() % 100;
        b = rand() % 100;
        cout << a << "," << b << endl;
        tree->insert(a, new int(b), true);
        tree->show(cout);
    }

    cout << "Node count: " << tree->count() << endl;
    cout << "Depth: " << tree->depth() << endl;

    cout << "Diagram:" << endl;
    tree->show(cout);

    /*
    cout << "Access elements:" << endl;
    for(int i = 0; i < 10; i ++) {
        int key = keys[rand() % num_keys];
        int* val = tree->get(key);
        if(val)
            cout << key << ": " << *val << endl;
        else
            cout << key << ": nullptr" << endl; // This should never happen in testing.
    }

    cout << "Search tree:" << endl;
    for(int i = 0; i < 10; i ++) {
        const int* val;
        if(i >= num_keys) {
            temp = rand() % 100;
            val = &temp;
        }
        else {
            val = tree->get(keys[i]);
        }
        int result;
        bool found = tree->search(result, val);
        if(found)
            cout << *val << ": " << result << endl;
        else
            cout << *val << ": not found" << endl;
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

    */

    cout << "Node count: " << tree->count() << endl;
    cout << "Depth: " << tree->depth() << endl;
}

int main()
{
    srand(16);

    cout << "Building tree" << endl;

    Tree<int>* tree = new AvlTree<int>();
    Tree<int>* tree2 = new BinaryTree<int>();

    cout << "Start testing" << endl;

    test(tree);
    test(tree2);
    
    cout << "Test end, deleting" << endl;

    delete tree;

    cout << "Run complete." << endl;

    return 0;
}
