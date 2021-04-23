

// For the sake of my IDE
#include "BPlusTree.h"

#include <assert.h>
#include <utility>
#include <iostream>
#include<fstream>

using std::swap;
//using std::cout;
//using std::endl;
//using std::cerr;

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
void BPlusNode<Value>::insert_node(int key, BPlusNode<Value>* value)
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
BPlusNode<Value>* BPlusNode<Value>::branch_split()
{
    if(m_size >= m_max_size)
    {
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
            right_node->next()->m_parent = right_node;
            for(int i = m_min_size + 1; i < m_size; i ++)
            {
                right_node->insert_node(m_keys[i], static_cast<BPlusNode<Value>**>(m_children)[i + 1]);
            }
        }
        m_size = m_min_size;

        if(m_parent)
        {
            m_parent->insert_node(right_node->min_key(), right_node);
            return m_parent->branch_split();
        }
        else {
            // Make new root node.
            BPlusNode<Value>* new_root = new BPlusNode<Value>();
            m_parent = new_root;
            new_root->init(false, m_max_size);
            new_root->next() = this;
            new_root->insert_node(right_node->min_key(), right_node);
            return new_root;
        }
    }
    return nullptr;
}

template <class Value>
Value* BPlusTree<Value>::get(int key) const
{
    // cout << "GET" << endl;
    if(!m_root) return nullptr;
    int index;
    BPlusNode<Value>* leaf = m_root->find(index, key);
    if(index == -1) return nullptr;
    // cout << "No early ret" << endl;

    return leaf->get(index);
}

template <class Value>
bool BPlusTree<Value>::insert(int key, Value *value, bool overwrite)
{
    // cout << "INSERT" << endl;
    // Base case, create the root node.
    if(!m_root)
    {
        m_root = new BPlusNode<Value>();
        m_root->init(true, m_node_size);
        m_root->insert(key, value);
        m_node_count = 1;
        return true;
    }
    
    // cout << "FIND INSERT NODE" << endl;
    int index;
    BPlusNode<Value>* leaf = m_root->find(index, key);

    // cout << "INDEX " << index << endl;
    // Case in which node already exists.
    if(index != -1)
    {
        assert(leaf->m_is_leaf);
        // Position is full
        if(leaf->get(index))
        {
            if(overwrite) {
                delete leaf->get(index);
                leaf->get(index) = value;
            }
            return overwrite;
        }
        // Position used to be occupied but now is empty
        else {
            leaf->get(index) = value;
            m_node_count ++;
            return true;
        }
    }

    // cout << "INSERT INTO LEAF" << endl;
    leaf->insert(key, value);

    // Split
    // cout << "INSERT INTO SPLIT" << endl;
    BPlusNode<Value>* new_root = leaf->branch_split();
    if(new_root)
    {
        m_root = new_root;
        m_depth++;
    }

    m_node_count++;

    std::ofstream f;
    f.open("BPLUSTREE.txt", std::ios_base::app);
    f << '"';
    show(f);
    f << "\",";
    f.close();

    // cout << "COMPLETE" << endl;
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

    m_node_count --;

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
                out = node->m_keys[i];
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

template <class Value>
BPlusTree<Value>& BPlusTree<Value>::operator=(const BPlusTree<Value>& src)
{
    delete m_root;
    m_depth = src.m_depth;
    m_node_count = src.m_node_count;
    m_node_size = src.m_node_size;
    if(src.m_root) {
        m_root = new BPlusNode<Value>(*src.m_root);
    }
    else
        m_root = nullptr;
        
    BPlusNode<Value>* temp = m_root;
    while(temp)
    {
        if(temp->m_is_leaf)
        {
            temp = temp->compute_right_node();
        }
        else {
            temp = temp->next();
        }
    }
    
    return *this;
}

template <class Value>
BPlusTree<Value>::BPlusTree(const BPlusTree<Value>& src)
{
    m_depth = src.m_depth;
    m_node_count = src.m_node_count;
    m_node_size = src.m_node_size;
    if(src.m_root) {
        m_root = new BPlusNode<Value>(*src.m_root);
    }
    else
        m_root = nullptr;
    
    BPlusNode<Value>* temp = m_root;
    while(temp)
    {
        if(temp->m_is_leaf)
        {
            temp = temp->compute_right_node();
        }
        else {
            temp = temp->next();
        }
    }

    // show(cout);
    // cout << endl;
}

template<class Value>
BPlusNode<Value>* BPlusNode<Value>::compute_right_node()
{
    assert(m_is_leaf);
    
    BPlusNode<Value>* curr = m_parent;

    // Case already in node.
    while(curr)
    {
        for(int i = 0; i < curr->m_size; i ++)
        {
            if(static_cast<BPlusNode<Value>**>(curr->m_children)[i] == this)
            {
                next() = static_cast<BPlusNode<Value>**>(curr->m_children)[i+1]->left();
                return next();
            }
        }
        curr = curr->m_parent;
    }

    return nullptr;
}

template<class Value>
BPlusNode<Value>* BPlusNode<Value>::left()
{
    BPlusNode<Value>* temp = this;
    while(!temp->m_is_leaf)
    {
        temp = temp->next();
    }
    return temp;
}

template <class Value>
void BPlusTree<Value>::clear()
{
    if(m_root) {
        delete m_root;
        m_root = nullptr;
        m_depth = 0;
        m_node_count = 0;
    }
}

}

