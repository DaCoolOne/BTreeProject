#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "BTree.h"
#include "BPlusTree.h"
#include "AvlTree.h"
#include "BinaryTree.h"

#include <iostream>
using std::cout;
using std::endl;

using namespace b_tree;

const int NUM_KEYS = 20;
const int KEYS[NUM_KEYS] = {30,1,21,300,23,70,4,16,76,3,32,95,45,10,-10,0,12,40,120,15};

TEST_CASE("Binary Constructor", "[Binary]")
{
    BinaryTree<int> tree;
    
    SECTION("DEFAULT CONSTRUCTOR")
    {
        CHECK(tree.count() == 0);
        CHECK(tree.depth() == 0);
    }
    
    for(int i = 0; i < NUM_KEYS; i ++)
        REQUIRE(tree.insert(KEYS[i], new int(KEYS[i])));
    
    SECTION("COPY CONSTRUCTOR")
    {
        BinaryTree<int> copy(tree);
        CHECK(tree.depth() == copy.depth());
        CHECK(tree.count() == copy.count());
        for(int i = 0; i < NUM_KEYS; i ++)
            CHECK(*tree.get(KEYS[i]) == *copy.get(KEYS[i]));
        
        copy.insert(KEYS[0], new int(KEYS[1]), true);
        CHECK(*tree.get(KEYS[0]) != *copy.get(KEYS[0]));
    }

    SECTION("COPY ASSIGNMENT")
    {
        BinaryTree<int> copy = tree;
        CHECK(tree.depth() == copy.depth());
        CHECK(tree.count() == copy.count());
        for(int i = 0; i < NUM_KEYS; i ++)
            CHECK(*tree.get(KEYS[i]) == *copy.get(KEYS[i]));
        
        copy.insert(KEYS[0], new int(KEYS[1]), true);
        CHECK(*tree.get(KEYS[0]) != *copy.get(KEYS[0]));
    }
}

TEST_CASE("Binary Operations", "[Binary]")
{
    BinaryTree<int> tree;
    
    for(int i = 0; i < NUM_KEYS; i ++)
        tree.insert(KEYS[i], new int(KEYS[i]));
    
    SECTION("LENGTH")
    {
        CHECK(tree.count() == NUM_KEYS);
        CHECK(tree.depth() > 0);
    }
    
    SECTION("GET")
    {
        for(int i = 0; i < NUM_KEYS; i ++)
            CHECK(*tree.get(KEYS[i]) == KEYS[i]);
    }
    SECTION("SEARCH")
    {
        int out;
        for(int i = 0; i < NUM_KEYS; i ++)
        {
            bool success = tree.search(out, &KEYS[i]);
            CHECK(success);
            CHECK(out == KEYS[i]);
        }
        int* f = new int(-100);
        CHECK(!tree.search(out, f));
        delete f;
    }
    SECTION("REMOVE")
    {
        for(int i = 0; i < NUM_KEYS; i ++)
        {
            CHECK(*tree.get(KEYS[i]) == KEYS[i]);
            bool res = tree.remove(KEYS[i]);
            CHECK(res);
            CHECK(!tree.get(KEYS[i]));
        }
    }
}

TEST_CASE("Avl Constructor", "[AVL]")
{
    AvlTree<int> tree;
    
    SECTION("DEFAULT CONSTRUCTOR")
    {
        CHECK(tree.count() == 0);
        CHECK(tree.depth() == 0);
    }
    
    for(int i = 0; i < NUM_KEYS; i ++)
        tree.insert(KEYS[i], new int(KEYS[i]));
    
    SECTION("COPY CONSTRUCTOR")
    {
        AvlTree<int> copy(tree);
        CHECK(tree.depth() == copy.depth());
        CHECK(tree.count() == copy.count());
        for(int i = 0; i < NUM_KEYS; i ++)
            CHECK(*tree.get(KEYS[i]) == *copy.get(KEYS[i]));
        
        copy.insert(KEYS[0], new int(KEYS[1]), true);
        CHECK(*tree.get(KEYS[0]) != *copy.get(KEYS[0]));
    }

    SECTION("COPY ASSIGNMENT")
    {
        AvlTree<int> copy = tree;
        CHECK(tree.depth() == copy.depth());
        CHECK(tree.count() == copy.count());
        for(int i = 0; i < NUM_KEYS; i ++)
            CHECK(*tree.get(KEYS[i]) == *copy.get(KEYS[i]));
        
        copy.insert(KEYS[0], new int(KEYS[1]), true);
        CHECK(*tree.get(KEYS[0]) != *copy.get(KEYS[0]));
    }
}

TEST_CASE("Avl Operations", "[AVL]")
{
    AvlTree<int> tree;
    
    for(int i = 0; i < NUM_KEYS; i ++)
        tree.insert(KEYS[i], new int(KEYS[i]));
    
    SECTION("LENGTH")
    {
        CHECK(tree.count() == NUM_KEYS);
        CHECK(tree.depth() > 0);
    }
    
    SECTION("GET")
    {
        for(int i = 0; i < NUM_KEYS; i ++)
            CHECK(*tree.get(KEYS[i]) == KEYS[i]);
    }
    SECTION("SEARCH")
    {
        int out;
        for(int i = 0; i < NUM_KEYS; i ++)
        {
            bool success = tree.search(out, &KEYS[i]);
            CHECK(success);
            CHECK(out == KEYS[i]);
        }
        int* f = new int(-100);
        CHECK(!tree.search(out, f));
        delete f;
    }
    SECTION("REMOVE")
    {
        for(int i = 0; i < NUM_KEYS; i ++)
        {
            CHECK(*tree.get(KEYS[i]) == KEYS[i]);
            bool res = tree.remove(KEYS[i]);
            CHECK(res);
            CHECK(!tree.get(KEYS[i]));
        }
    }
}

TEST_CASE("BTree Constructor", "[BTree]")
{
    BTree<int> tree;
    
    SECTION("DEFAULT CONSTRUCTOR")
    {
        CHECK(tree.count() == 0);
        CHECK(tree.depth() == 0);
    }
    
    for(int i = 0; i < NUM_KEYS; i ++)
        tree.insert(KEYS[i], new int(KEYS[i]));
    
    SECTION("COPY CONSTRUCTOR")
    {
        BTree<int> copy(tree);
        CHECK(tree.depth() == copy.depth());
        CHECK(tree.count() == copy.count());
        for(int i = 0; i < NUM_KEYS; i ++)
            CHECK(*tree.get(KEYS[i]) == *copy.get(KEYS[i]));
        
        copy.insert(KEYS[0], new int(KEYS[1]), true);
        CHECK(*tree.get(KEYS[0]) != *copy.get(KEYS[0]));
    }

    SECTION("COPY ASSIGNMENT")
    {
        BTree<int> copy = tree;
        CHECK(tree.depth() == copy.depth());
        CHECK(tree.count() == copy.count());
        for(int i = 0; i < NUM_KEYS; i ++)
            CHECK(*tree.get(KEYS[i]) == *copy.get(KEYS[i]));
        
        copy.insert(KEYS[0], new int(KEYS[1]), true);
        CHECK(*tree.get(KEYS[0]) != *copy.get(KEYS[0]));
    }
}

TEST_CASE("BTree Operations", "[BTree]")
{
    BTree<int> tree;
    
    for(int i = 0; i < NUM_KEYS; i ++)
        tree.insert(KEYS[i], new int(KEYS[i]));
    
    SECTION("LENGTH")
    {
        CHECK(tree.count() == NUM_KEYS);
        CHECK(tree.depth() > 0);
    }
    
    SECTION("GET")
    {
        for(int i = 0; i < NUM_KEYS; i ++)
            CHECK(*tree.get(KEYS[i]) == KEYS[i]);
    }
    SECTION("SEARCH")
    {
        int out;
        for(int i = 0; i < NUM_KEYS; i ++)
        {
            bool success = tree.search(out, &KEYS[i]);
            CHECK(success);
            CHECK(out == KEYS[i]);
        }
        int* f = new int(-100);
        CHECK(!tree.search(out, f));
        delete f;
    }
    SECTION("REMOVE")
    {
        for(int i = 0; i < NUM_KEYS; i ++)
        {
            CHECK(*tree.get(KEYS[i]) == KEYS[i]);
            bool res = tree.remove(KEYS[i]);
            CHECK(res);
            CHECK(!tree.get(KEYS[i]));
        }
    }
}

TEST_CASE("BPlus Constructor", "[BPlus]")
{
    BPlusTree<int> tree;
    
    SECTION("DEFAULT CONSTRUCTOR")
    {
        CHECK(tree.count() == 0);
        CHECK(tree.depth() == 0);
    }

    for(int i = 0; i < NUM_KEYS; i ++)
        tree.insert(KEYS[i], new int(KEYS[i]));
    
    SECTION("ACCESSORS")
    {
        for(int i = 0; i < NUM_KEYS; i ++)
            REQUIRE(*tree.get(KEYS[i]) == KEYS[i]);
    }

    SECTION("COPY CONSTRUCTOR")
    {
        BPlusTree<int> copy(tree);
        SECTION("COPY QD CHECK")
        {
            CHECK(tree.depth() == copy.depth());
            CHECK(tree.count() == copy.count());
        }
        SECTION("ACCURATE COPY")
        {
            for(int i = 0; i < NUM_KEYS; i ++)
                CHECK(*tree.get(KEYS[i]) == *copy.get(KEYS[i]));
        }
        
        SECTION("DEEP COPY")
        {
            copy.insert(KEYS[0], new int(KEYS[1]), true);
            CHECK(*tree.get(KEYS[0]) != *copy.get(KEYS[0]));
        }
    }

    SECTION("COPY ASSIGNMENT")
    {
        BPlusTree<int> copy = tree;
        CHECK(tree.depth() == copy.depth());
        CHECK(tree.count() == copy.count());
        for(int i = 0; i < NUM_KEYS; i ++)
            CHECK(*tree.get(KEYS[i]) == *copy.get(KEYS[i]));
        
        copy.insert(KEYS[0], new int(KEYS[1]), true);
        CHECK(*tree.get(KEYS[0]) != *copy.get(KEYS[0]));
    }
}

TEST_CASE("BPlus Operations", "[BPlus]")
{
    BPlusTree<int> tree;
    
    for(int i = 0; i < NUM_KEYS; i ++)
        tree.insert(KEYS[i], new int(KEYS[i]));
    
    SECTION("LENGTH")
    {
        CHECK(tree.count() == NUM_KEYS);
        CHECK(tree.depth() > 0);
    }
    
    SECTION("GET")
    {
        for(int i = 0; i < NUM_KEYS; i ++)
            CHECK(*tree.get(KEYS[i]) == KEYS[i]);
    }
    SECTION("SEARCH")
    {
        int out;
        for(int i = 0; i < NUM_KEYS; i ++)
        {
            bool success = tree.search(out, &KEYS[i]);
            CHECK(success);
            CHECK(out == KEYS[i]);
        }
        int* f = new int(-100);
        CHECK(!tree.search(out, f));
        delete f;
    }
    SECTION("REMOVE")
    {
        for(int i = 0; i < NUM_KEYS; i ++)
        {
            CHECK(*tree.get(KEYS[i]) == KEYS[i]);
            bool res = tree.remove(KEYS[i]);
            CHECK(res);
            CHECK(!tree.get(KEYS[i]));
        }
    }
}

