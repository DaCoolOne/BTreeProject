

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include "BaseTree.h"
namespace b_tree {

template <class Key, class Value>
struct Node {
    Key m_key;
    Value* m_value;

    Node<Key, Value>* m_left;
    Node<Key, Value>* m_right;

    // Deals with instantiation and deletion.
    Node() { m_left = nullptr; m_right = nullptr; }
    // Recursive node deletion.
    ~Node() { delete m_value; delete m_left; delete m_right; }

    // Used when removing a node to avoid deleting subtrees.
    void clear() { m_left = nullptr; m_right = nullptr; }
};

template <class Key, class Value>
class BinaryTree : public Tree<Key, Value>
{
    private:

        // Private data structures.
        int m_node_count;
        int m_node_depth;
        Node<Key, Value>* m_root;
    
    public:
        BinaryTree() { m_node_depth = 0; m_node_count = 0; m_root = nullptr; }
        ~BinaryTree() override { delete m_root; }

        bool insert(Key key, Value *value, bool overwrite = false) override;

        bool remove(Key key) override;

        Value* get(Key key) const override;

        bool search(Key& out, Value* value) override;

        int count() const override { return m_node_count; }

        int depth() const override { return m_node_depth; }
};

}

#include "BinaryTree.hpp"

#endif