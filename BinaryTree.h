

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <iostream>
#include "BaseTree.h"
namespace b_tree {

template <class Value>
struct Node {
    int m_key;
    Value* m_value;

    Node<Value>* m_left;
    Node<Value>* m_right;

    // Deals with instantiation and deletion.
    Node() { m_left = nullptr; m_right = nullptr; }
    // Recursive node deletion.
    ~Node() { delete m_value; delete m_left; delete m_right; }

    // Used when removing a node to avoid deleting subtrees.
    void clear() { m_left = nullptr; m_right = nullptr; }
    
    // Used for search.
    bool search(int& out, const Value* value);
};

template <class Value>
class BinaryTree : public Tree<Value>
{
    private:

        // Private data structures.
        int m_node_count;
        int m_node_depth;
        Node<Value>* m_root;
    
    public:
        BinaryTree() { m_node_depth = 0; m_node_count = 0; m_root = nullptr; }
        ~BinaryTree() override { delete m_root; }

        bool insert(int key, Value *value, bool overwrite = false) override;

        bool remove(int key) override;

        Value* get(int key) const override;

        bool search(int& out, const Value* value) override;

        int count() const override { return m_node_count; }

        int depth() const override { return m_node_depth; }

        void show(std::ostream &out) override;
};

}

#include "BinaryTree.hpp"

#endif