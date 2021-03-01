

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <iostream>
#include "BaseTree.h"
namespace b_tree {

template <class Value>
struct BinaryNode {
    int m_key;
    Value* m_value;

    BinaryNode<Value>* m_left;
    BinaryNode<Value>* m_right;

    // Deals with instantiation and deletion.
    BinaryNode() { m_left = nullptr; m_right = nullptr; }
    // Recursive node deletion.
    ~BinaryNode() { delete m_value; delete m_left; delete m_right; }

    // Used when removing a node to avoid deleting subtrees.
    void clear() { m_left = nullptr; m_right = nullptr; }
    
    // Used for search.
    bool search(int& out, const Value* value) const;
};

template <class Value>
class BinaryTree : public Tree<Value>
{
    private:

        // Private data structures.
        int m_node_count;
        int m_node_depth;
        BinaryNode<Value>* m_root;
    
    public:
        BinaryTree() { m_node_depth = 0; m_node_count = 0; m_root = nullptr; }
        ~BinaryTree() override { delete m_root; }

        bool insert(int key, Value *value, bool overwrite = false) override;

        bool remove(int key) override;

        Value* get(int key) const override;

        bool search(int& out, const Value* value) const override;

        int count() const override { return m_node_count; }

        int depth() const override { return m_node_depth; }

        void show(std::ostream &out) override;
};

}

#include "BinaryTree.hpp"

#endif