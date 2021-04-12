#ifndef B_TREE_H
#define B_TREE_H

#include "BaseTree.h"

namespace b_tree {

// Default size of the nodes.
// E.g, a value of 4 will create 2-4 trees.
const int DEFAULT_B_NODE_SIZE = 4;

template <class Value>
struct BNode {
    int m_size;
    int m_min_size;
    int m_max_size;

    bool m_size_set;
    int* m_keys;
    Value** m_values;
    BNode<Value>** m_children;
    BNode<Value>* m_parent;

    BNode() {
        m_keys = nullptr;
        m_children = nullptr;
        m_values = nullptr;
        m_parent = nullptr;
        m_size = 0;
        m_size_set = false;
    }
    ~BNode() {
        delete[] m_keys;
        delete[] m_children;

        for(int i = 0; i < m_size; i ++)
            delete m_values[i];
        
        delete[] m_values;
    }

    // Deletes all children recursively
    void deleteChildren();

    // Sets the left child of the node.
    void setLeftChild(BNode<Value>* child);

    // Sets the size of the node since it can't necessarily be set at initialization.
    void setSize(int size) {
        // Ensure we don't set the node's size twice.
        if(!m_size_set) {
            // Allocate an extra spot in the node to allow for easier splitting of the node.
            m_keys = new int[size+1];
            m_values = new Value*[size+1];
            m_children = new BNode<Value>*[size+2];
            m_size = 0;
            m_max_size = size;
            m_min_size = (size + 1) / 2;
            m_size_set = true;
        }
    };

    Value* getValueAtKey(int key) const;

    // Gets the node that store a key and returns the index of that key (or -1 if the key is not found).
    int getNodeIndex(BNode<Value>*& out, int key);
    int getNodeIndex(const BNode<Value>*& out, int key) const;

    // Search the tree recursively for a value.
    bool search(int& out, const Value* val) const;

    // Returns the new root if the root node changes, otherwise it
    // returns nullptr
    BNode<Value>* split();

    // Inserts the key into the node, shifting over elements as needed.
    void insert(int new_key, Value* value, BNode<Value>* child);
};

template<class Value>
class BTree : public Tree<Value> {

private:
    int m_node_depth;
    int m_node_count;

    int m_node_default;

    BNode<Value>* m_root;

public:
    BTree() { m_node_count = 0; m_node_depth = 0; m_root = nullptr; m_node_default = DEFAULT_B_NODE_SIZE; }
    BTree(int node_size) { m_node_count = 0; m_node_depth = 0; m_root = nullptr; m_node_default = node_size; }
    ~BTree() override { if(m_root) { m_root->deleteChildren(); delete m_root; } }

    bool insert(int key, Value *value, bool overwrite = false) override;

    bool remove(int key) override;

    Value* get(int key) const override;

    bool search(int& out, const Value* value) const override;

    int count() const override { return m_node_count; }

    int depth() const override { return m_node_depth; }

    void show(std::ostream &out) const override;
};

}

#include "BTree.hpp"

#endif