

// For the sake of my IDE
#include "BPlusTree.h"

#include <assert.h>
#include <utility>
#include <iostream>

using std::swap;
using std::cout;
using std::endl;
using std::cerr;

namespace b_tree {

template <class Value>
Value*& BPlusNode<Value>::get(int index) const
{
    assert(m_is_leaf && 0 <= index && index < m_size);
    return static_cast<Value**>(m_children)[index + 1];
}

// Exact copy of function below, see other version for documentation.
template <class Value>
BPlusNode<Value>* BPlusNode<Value>::find(int& index, int key)
{
    if(m_is_leaf)
    {
        for(int i = 0; i < m_size; ++i)
        {
            if(m_keys[i] == key) {
                index = i;
                return this;
            }
        }
        index = -1;
        return this;
    }
    else {
        // Go backwards to find first key that matches.
        for(int i = m_size - 1; i >= 0; --i)
        {
            if(key >= m_keys[i])
            {
                BPlusNode<Value>* node = static_cast<BPlusNode<Value>**>(m_children)[i + 1];
                return node->find(index, key);
            }
        }
        BPlusNode<Value>* lowest = next();
        return lowest->find(index, key);
    }
}

// Used for finding specific nodes in the tree where values either exist or should go.
template <class Value>
const BPlusNode<Value>* BPlusNode<Value>::find(int& index, int key) const
{
    if(m_is_leaf)
    {
        for(int i = 0; i < m_size; ++i)
        {
            if(m_keys[i] == key) {
                index = i;
                return this;
            }
        }
        index = -1;
        return this;
    }
    else {
        // Go backwards to find first key that matches.
        for(int i = m_size - 1; i >= 0; --i)
        {
            if(key >= m_keys[i])
            {
                BPlusNode<Value>* node = static_cast<BPlusNode<Value>**>(m_children)[i + 1];
                return node->find(index, key);
            }
        }
        BPlusNode<Value>* lowest = next();
        return lowest->find(index, key);
    }
}

// Precondition, new value does not exist in the node already.
template <class Value>
void BPlusNode<Value>::insert(int key, BPlusNode<Value>* value)
{
    value->m_parent = this;

    // cerr << "Perform insertion of node, key " << key << endl;
    // Insert data using this fancy-pants swap-based algorithm.
    for(int i = 0; i <= m_size; i ++) {
        if(key < m_keys[i] || i == m_size)
        {
            swap(m_keys[i], key);
            swap(static_cast<BPlusNode<Value>**>(m_children)[i + 1], value);
        }
    }

    // Make size bigger
    ++m_size;
}

// Precondition, new value does not exist in the node already.
template <class Value>
void BPlusNode<Value>::insert(int key, Value* value)
{
    // cerr << "Perform insertion of key " << key << endl;
    // Insert data using this fancy-pants swap-based algorithm.
    for(int i = 0; i <= m_size; i ++) {
        if(key < m_keys[i] || i == m_size)
        {
            swap(m_keys[i], key);
            swap(static_cast<Value**>(m_children)[i + 1], value);
        }
    }

    // Make size bigger
    ++m_size;
}

template <class Value>
int BPlusNode<Value>::min_key()
{
    if(m_is_leaf)
        return m_keys[0];
    else
        return next()->min_key();
}

template <class Value>
BPlusNode<Value>* BPlusNode<Value>::branch_split(int& nodes)
{
    if(m_size >= m_max_size)
    {
        ++ nodes;

        BPlusNode<Value>* right_node = new BPlusNode<Value>();
        right_node->init(m_is_leaf, m_max_size);

        if(m_is_leaf) {
            // Copy the next pointer node.
            right_node->next() = next();
            
            for(int i = m_min_size; i < m_size; i ++)
            {
                right_node->insert(m_keys[i], get(i));
            }
            next() = right_node;
        }
        else {
            right_node->next() = static_cast<BPlusNode<Value>**>(m_children)[m_min_size + 1];
            for(int i = m_min_size + 1; i < m_size; i ++)
            {
                right_node->insert(m_keys[i], static_cast<BPlusNode<Value>**>(m_children)[i + 1]);
            }
        }
        m_size = m_min_size;

        if(m_parent)
        {
            ++ nodes;

            m_parent->insert(right_node->min_key(), right_node);
            return m_parent->branch_split(nodes);
        }
        else {
            // Make new root node.
            BPlusNode<Value>* new_root = new BPlusNode<Value>();
            m_parent = new_root;
            new_root->init(false, m_max_size);
            new_root->next() = this;
            new_root->insert(right_node->min_key(), right_node);
            return new_root;
        }
    }
    return nullptr;
}

template <class Value>
Value* BPlusTree<Value>::get(int key) const
{
    if(!m_root) return nullptr;
    int index;
    BPlusNode<Value>* leaf = m_root->find(index, key);

    if(index == -1) return nullptr;

    return leaf->get(index);
}

template <class Value>
bool BPlusTree<Value>::insert(int key, Value *value, bool overwrite)
{
    // Base case, create the root node.
    if(!m_root)
    {
        m_root = new BPlusNode<Value>();
        m_root->init(true, m_node_size);
        m_root->insert(key, value);
        return true;
    }
    
    int index;
    BPlusNode<Value>* leaf = m_root->find(index, key);

    // Case in which node already exists.
    if(index != -1)
    {
        // Position is full
        if(leaf->get(index))
        {
            if(overwrite) {
                delete m_root->get(index);
                leaf->get(index) = value;
            }
            return overwrite;
        }
        // Position used to be occupied but now is empty
        else {
            leaf->get(index) = value;
            return true;
        }
    }

    leaf->insert(key, value);

    // Split
    BPlusNode<Value>* new_root = leaf->branch_split(m_node_count);
    if(new_root) m_root = new_root;

    return true;
}

template<class Value>
void _print_b_plus_node(std::ostream &out, BPlusNode<Value>* node)
{
    // cerr << "\nSIZE: " << node->m_size << " LEAF: " << node->m_is_leaf << '\n';
    for(int i = 0; i < node->m_size; i ++) {
        out << node->m_keys[i];
        
        if(i < node->m_size - 1) out << "|";
    }
    out << '(';
    for(int i = 0; i <= node->m_size; i ++) {
        if(!node->m_is_leaf)
        {
            // cerr << "EVEN FURTHER BEYOND!" << endl;
            _print_b_plus_node(out, static_cast<BPlusNode<Value>**>(node->m_children)[i]);
        }
        
        if(i < node->m_size) out << ",";
    }
    out << ')';
}

template <class Value>
bool BPlusTree<Value>::remove(int key)
{
    if(!m_root) return false;
    
    int index;
    BPlusNode<Value>* leaf = m_root->find(index, key);

    if(index == -1) return false;
    
    delete leaf->get(index);
    leaf->get(index) = nullptr;

    return true;
}

template <class Value>
bool BPlusTree<Value>::search(int& out, const Value* value) const
{
    if(!m_root) return false;

    // Start at root.
    BPlusNode<Value>* node = m_root;
    
    // Move to left-most leaf.
    while(!node->m_is_leaf) node = node->next();

    // Traverse tree until search value found or no more leaves.
    while(node) {
        for(int i = 0; i < node->m_size; i ++)
        {
            if(*node->get(i) == *value)
            {
                out = i; // node->m_keys[i];
                return true;
            }
        }

        // Go to next leaf
        node = node->next();
    }

    return false;
}

template <class Value>
void BPlusTree<Value>::show(std::ostream &out) const
{
    if(m_root) _print_b_plus_node(out, m_root);
}

}

