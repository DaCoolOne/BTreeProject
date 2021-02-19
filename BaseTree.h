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
template <class Key, class Value>
class Tree
{
    public:
        // To add (not listed in abstract class):
        // Copy assignment constructor (preferably uses abstract Tree class)
        // Copy constructor (preferably uses abstract Tree class)

        // Returns true if insertion was successful. If the key
        // already exists, then the value is only changed if 
        // overwrite is true.
        virtual bool insert(Key key, Value *value, bool overwrite = false) = 0;

        // Removes and deletes the value located in the at the key.
        // Returns false if key does not exist.
        virtual bool remove(Key key) = 0;

        // Gets and returns a pointer to the value at the key, or nullptr if
        // the key does not exist in the tree.
        virtual Value* get(Key key) const = 0;

        // Finds the first key with a certain value.
        virtual bool const search(Key& out, Value* value) = 0;

        // Gets the number of nodes in the tree.
        virtual int count() const = 0;

        // Gets the number of nodes in the tree.
        virtual int depth() const = 0;

        // Some other potential ideas of things to add:
};

}
#endif