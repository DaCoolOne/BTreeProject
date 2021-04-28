

#include <string>
#include <fstream>
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
const int KEYS_IN_USE = 1000;
const int NUM_KEYS = 10000;
const int MAX_RUNS = 100;

void test(Tree<int>* tree, std::ostream& res_stream)
{
    Timer timer;
    Timer runtime;
    Average<double> grow_shrink_test;
    Average<double> get_test;
    Average<double> ideal_get_test;
    Average<double> search_test;
    Average<double> sequential_test;

    Average<double> runtime_avg;

    int* out;
    int temp = 0, out2;
    double time;

    // Construct the key array
    int keys[NUM_KEYS];
    for(int i = 0; i < NUM_KEYS; i ++)
    {
        keys[i] = i;
    }

    for(int runs = 0; runs < MAX_RUNS;)
    {
        cout << "Run " << (++runs) << endl;
        time = 0;
        runtime.start();

        // Grow shrink test:
        for(int j = 0; j < 100; j ++)
        {
            shuffle(keys, NUM_KEYS);
            timer.start();
            for(int i = 0; i < KEYS_IN_USE; i ++)
            {
                tree->insert(keys[i], new int(i));
            }
            shuffle(keys, KEYS_IN_USE);
            for(int i = 0; i < KEYS_IN_USE; i ++)
            {
                tree->remove(keys[i]);
            }
            time += timer.elapsed();
        }
        grow_shrink_test.add(time);

        // Access test:
        shuffle(keys, NUM_KEYS);
        for(int i = 0; i < KEYS_IN_USE; i ++)
        {
            tree->insert(keys[i], new int(i));
        }
        timer.start();
        for(int j = 0; j < 100; j ++) {
            for(int i = 0; i < KEYS_IN_USE; i ++)
            {
                out = tree->get(keys[i]);
                temp ^= *out;
            }
        }
        get_test.add(timer.elapsed());

        // Idealized access test.
        tree->clear();
        shuffle(keys, NUM_KEYS);
        for(int i = 0; i < KEYS_IN_USE; i ++)
        {
            tree->insert(keys[i], new int(keys[i]));
        }
        timer.start();
        for(int j = 0; j < 100; j ++) {
            for(int i = 0; i < KEYS_IN_USE; i ++)
            {
                out = tree->get(keys[i]);
                temp ^= *out;
            }
        }
        ideal_get_test.add(timer.elapsed());
        
        timer.start();
        for(int j = 0; j < 100; j ++) {
            shuffle(keys, KEYS_IN_USE);
            for(int i = 0; i < KEYS_IN_USE; i ++)
            {
                tree->search(out2, keys + i);
                temp ^= out2;
            }
        }
        search_test.add(timer.elapsed());

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
                out = tree->get(i);
                temp ^= *out;
            }
        }
        sequential_test.add(timer.elapsed());

        runtime_avg.add(runtime.elapsed());
    }
    
    std::string unit = " " + timer.units();
    res_stream << "Grow/shrink test: " << grow_shrink_test.get() << unit << endl;
    res_stream << "Access test: " << get_test.get() << unit << endl;
    res_stream << "Idealized access test: " << ideal_get_test.get() << unit << endl;
    res_stream << "Sequential test: " << sequential_test.get() << unit << endl;
    res_stream << "Search test: " << search_test.get() << unit << endl;

    res_stream << "Total runtime: " << runtime_avg.get() << unit << endl;

    res_stream << temp << endl; // Just to be extra sure that the temp doesn't get optimized out.
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    std::ofstream fout("output.txt");

    cout << "[BEGIN]\n" << endl;

    const int NUM_TREES = 4;
    Tree<int>* trees[NUM_TREES] = {new b_tree::BinaryTree<int>(),new b_tree::AvlTree<int>(),new b_tree::BTree<int>(),new b_tree::BPlusTree<int>()};

    std::string tree_names[NUM_TREES] = {"BinaryTree","AvlTree","BTree","BPlusTree"};

    for(int i = 0; i < NUM_TREES; i++)
    {
        fout << "Test " << tree_names[i] << endl;
        cout << "Test " << tree_names[i] << endl;

        for(int j = 0; j < argc; j ++)
        {
            std::string arg(argv[j]);
            test(trees[i], fout);
        }

        delete trees[i];

        cout << endl;
    }

    cout << "Run complete." << endl;

    fout.close();

    return 0;
}
