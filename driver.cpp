

#include <iostream>
#include <cstdlib>
#include "BinaryTree.h"
using b_tree::Tree;
using b_tree::BinaryTree;

using std::cout;
using std::endl;
using std::rand;

void test(Tree<int, int>* tree)
{
    const static int keys[] = { 30,12,300,23,1,70,40 };
    const static int num_keys = 7;

    cout << "Insertion of items" << endl;
    for(int i = 0; i < num_keys; i ++)
        tree->insert(keys[i], new int(rand() % 100));

    cout << "Node count: " << tree->count() << endl;
    cout << "Depth: " << tree->depth() << endl;

    cout << "Access random elements:" << endl;
    for(int i = 0; i < 10; i ++) {
        cout << *(tree->get(keys[rand() % num_keys])) << endl;
    }
}

int main()
{
    cout << "Building tree" << endl;

    Tree<int, int>* tree = new BinaryTree<int, int>();

    cout << "Start testing" << endl;

    test(tree);
    
    cout << "Test end, deleting" << endl;

    delete tree;

    cout << "Run complete." << endl;

    return 0;
}
