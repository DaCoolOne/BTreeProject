#ifndef BASE_TREE_H
#define BASE_TREE_H

#include <iterator>
namespace b_tree {

/*
// Base abstract class for Node. (May need for iteration or finding nodes or something idk)
template <class Key, class Value>
class Node
{

};
*/

// Base abstract class for trees.
template <class Value>
class Tree
{
    public:
        virtual ~Tree() {};

        // To add (not listed in abstract class):
        // Copy assignment constructor (preferably uses abstract Tree class)
        // Copy constructor (preferably uses abstract Tree class)

        // Returns true if insertion was successful. If the key
        // already exists, then the value is only changed if 
        // overwrite is true.
        virtual bool insert(int key, Value *value, bool overwrite = false) = 0;

        // Removes and deletes the value located in the at the key.
        // Returns false if key does not exist.
        virtual bool remove(int key) = 0;

        // Gets and returns a pointer to the value at the key, or nullptr if
        // the key does not exist in the tree.
        virtual Value* get(int key) const = 0;

        // Finds the first key with a certain value.
        virtual bool search(int& out, const Value* value) = 0;

        // Gets the number of nodes in the tree.
        virtual int count() const = 0;

        // Gets the number of nodes in the tree.
        virtual int depth() const = 0;

        // Print the tree.
        virtual void show(std::ostream &out) { }

        // Some other potential ideas of things to add:
};

}
#endif