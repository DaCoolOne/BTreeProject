#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "BaseTree.h"
#include <iostream>

namespace b_tree {

template <class Value>
// The AvlNode is a bit more sophisicated than the BinaryTreeNode because it needs to 
struct AvlNode {
    int m_key;
    Value* m_value;

    AvlNode<Value>* m_parent;
    AvlNode<Value>* m_left;
    AvlNode<Value>* m_right;

    short m_left_depth;
    short m_right_depth;

    // Deals with instantiation and deletion.
    AvlNode() { m_parent = nullptr; m_left = nullptr; m_right = nullptr; m_left_depth = 0; m_right_depth = 0; }
    // Recursive node deletion.
    ~AvlNode() { delete m_value; delete m_left; delete m_right; }

    AvlNode<Value>* rotate(bool r1, bool r2);

    // Used when removing a node to avoid deleting subtrees.
    void clear() { m_left = nullptr; m_right = nullptr; }
    
    Value* get(int key) const;

    // Used for search.
    bool search(int& out, const Value* value) const;
};

template<class Value>
class AvlTree : public Tree<Value>
{

private:
    AvlNode<Value>* m_root;

    int m_node_depth;
    int m_node_count;

public:
    AvlTree() { m_node_depth = 0; m_node_count = 0; m_root = nullptr; }

    bool insert(int key, Value *value, bool overwrite = false) override;

    // Removes and deletes the value located in the at the key.
    // Returns false if key does not exist.
    bool remove(int key) override;

    // Gets and returns a pointer to the value at the key, or nullptr if
    // the key does not exist in the tree.
    Value* get(int key) const override;

    // Finds the first key with a certain value.
    bool search(int& out, const Value* value) const override;

    // Gets the number of nodes in the tree.
    int count() const override { return m_node_count; }

    // Gets the number of nodes in the tree.
    int depth() const override { return m_node_depth; }

    // Print the tree.
    void show(std::ostream &out) override;

};

}

#include "AvlTree.hpp"

#endif