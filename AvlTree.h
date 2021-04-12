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
    ~AvlNode() { delete m_value; if(m_left) delete m_left; if(m_right) delete m_right; }

    // Perform a rotation on the current node.
    AvlNode<Value>* rotate(bool r1, bool r2);

    // Used when removing a node to avoid deleting subtrees.
    void clear() { m_left = nullptr; m_right = nullptr; }
    
    AvlNode<Value>* get(int key);

    // Shrinks m_left_depth to fit the size of the tree. DOES NOT GROW LEFT DEPTH
    void shrink_left_depth();
    // Shrinks m_right_depth to fit the size of the tree. DOES NOT GROW LEFT DEPTH
    void shrink_right_depth();

    // Assumes both subtrees balance factors are accurate.
    void recalculate_balance();

    // Finds the node to replace the current node if we want to delete this node.
    // Returns number of child nodes.
    short get_successor(AvlNode<Value>*& node) const;

    // Used for search.
    bool search(int& out, const Value* value) const;

    // Removes this node from the tree.
    AvlNode<Value>* pop(AvlNode<Value>*& root);

    void balance(AvlNode<Value>*& root);
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
    ~AvlTree() override { delete m_root; }

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
    int depth() const override { if(m_root) return max(m_root->m_left_depth, m_root->m_right_depth) + 1; else return 0; }

    // Print the tree.
    void show(std::ostream &out) const override;

};

}

#include "AvlTree.hpp"

#endif