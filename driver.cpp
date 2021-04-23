

#include <string>
#include <iostream>
#include <cstdlib>

#include "BinaryTree.h"
#include "AvlTree.h"
#include "BTree.h"
#include "BPlusTree.h"

#include "Timer.h"
#include "Average.h"

using b_tree::Tree;

using std::swap;
using std::cout;
using std::endl;
using std::rand;

template <typename T>
void shuffle(T arr[], int length)
{
    for(int i = length; i > 0; --i)
    {
        swap(arr[i - 1], arr[rand() % i]);
    }
}

template <typename T>
bool exists(T arr[], int length, T elmt)
{
    for(int i = 0; i < length; i ++)
        if(arr[i] == elmt)
            return true;
    return false;
}

void test(Tree<int>* tree)
{
    Timer timer;
    Timer runtime;
    Average<double> grow_shrink_test;
    Average<double> get_test;
    Average<double> ideal_get_test;
    Average<double> sequential_test;

    Average<int> keyg;
    Average<double> runtime_avg;

    int temp = 0;

    // Construct the key array
    srand(16);
    const int KEYS_IN_USE = 1000;
    const int NUM_KEYS = 5000;
    const int MAX_RUNS = 10;
    int keys[NUM_KEYS];
    for(int i = 0; i < NUM_KEYS; i ++)
    {
        keys[i] = i;
    }

    for(int runs = 0; runs < MAX_RUNS; runs++)
    {
        runtime.start();

        // Grow shrink test:
        timer.start();
        for(int j = 0; j < 100; j ++)
        {
            shuffle(keys, NUM_KEYS);
            for(int i = 0; i < KEYS_IN_USE; i ++)
            {
                tree->insert(keys[i], new int(rand()));
            }
            shuffle(keys, KEYS_IN_USE);
            for(int i = 0; i < KEYS_IN_USE; i ++)
            {
                tree->remove(keys[i]);
            }
        }
        grow_shrink_test.add(timer.elapsed());

        // Access test:
        shuffle(keys, NUM_KEYS);
        for(int i = 0; i < KEYS_IN_USE; i ++)
        {
            tree->insert(keys[i], new int(rand()));
        }
        timer.start();
        for(int j = 0; j < 100; j ++) {
            // shuffle(keys, KEYS_IN_USE);
            for(int i = 0; i < KEYS_IN_USE; i ++)
            {
                int* out = tree->get(keys[i]);
                temp ^= *out;
            }
        }
        get_test.add(timer.elapsed());

        // Idealized access test.
        tree->clear();
        shuffle(keys, NUM_KEYS);
        for(int i = 0; i < KEYS_IN_USE; i ++)
        {
            tree->insert(keys[i], new int(rand()));
        }
        timer.start();
        for(int j = 0; j < 100; j ++) {
            shuffle(keys, KEYS_IN_USE);
            for(int i = 0; i < KEYS_IN_USE; i ++)
            {
                int* out = tree->get(keys[i]);
                temp ^= *out;
            }
        }
        ideal_get_test.add(timer.elapsed());

        // Sequential insert test:
        tree->clear();
        for(int i = 0; i < KEYS_IN_USE; i ++)
        {
            tree->insert(i, new int(rand()));
        }

        timer.start();
        for(int j = 0; j < 100; j ++) {
            for(int i = 0; i < KEYS_IN_USE; i ++)
            {
                int* out = tree->get(i);
                temp ^= *out;
            }
        }
        sequential_test.add(timer.elapsed());

        runtime_avg.add(runtime.elapsed());
    }
    
    cout << "Grow/shrink test: " << grow_shrink_test.get() << 's' << endl;
    cout << "Access test: " << get_test.get() << 's' << endl;

    cout << "Total runtime: " << runtime_avg.get() << 's' << endl;

    cout << temp << endl; // Just to be extra sure that the temp doesn't get optimized out.
}

int main()
{
    srand(time(NULL));

    cout << "[BEGIN]\n" << endl;

    const int NUM_TREES = 4;
    Tree<int>* trees[NUM_TREES] = {new b_tree::BinaryTree<int>(),new b_tree::AvlTree<int>(),new b_tree::BTree<int>(),new b_tree::BPlusTree<int>()};

    std::string tree_names[NUM_TREES] = {"BinaryTree","AvlTree","BTree","BPlusTree"};

    for(int i = 0; i < NUM_TREES; i++)
    {
        cout << "Start testing " << tree_names[i] << endl;

        test(trees[i]);

        cout << "Test end, deleting" << endl << endl;

        delete trees[i];
    }

    cout << "Run complete." << endl;

    return 0;
}
