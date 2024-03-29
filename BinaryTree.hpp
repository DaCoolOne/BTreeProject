

#include <algorithm>

// Not strictly necessary, but used for type hinting.
#include "BinaryTree.h"
#include <iostream>

using std::max;
using std::endl;

namespace b_tree {

template<class Value>
BinaryNode<Value>::BinaryNode(const BinaryNode<Value>& source)
{
    m_value = new Value(*source.m_value);
    m_key = source.m_key;
    if(source.m_left)
        m_left = new BinaryNode<Value>(*source.m_left);
    else
        m_left = nullptr;
    if(source.m_right)
        m_right = new BinaryNode<Value>(*source.m_right);
    else
        m_right = nullptr;
}

template<class Value>
BinaryNode<Value>* BinaryNode<Value>::get(int key)
{
    if(m_key == key)
        return this;
    if(key < m_key)
        return m_left ? m_left->get(key) : nullptr;
    else
        return m_right ? m_right->get(key) : nullptr;
}
template<class Value>
BinaryNode<Value>* BinaryNode<Value>::get(int key) const
{
    if(m_key == key)
        return this;
    if(key < m_key)
        return m_left ? m_left->get(key) : nullptr;
    else
        return m_right ? m_right->get(key) : nullptr;
}

template<class Value>
Value* BinaryTree<Value>::get(int key) const
{
    if(!m_root) return nullptr;

    BinaryNode<Value>* node = m_root->get(key);

    if(!node) return nullptr;

    return node->m_value;
}

template<class Value>
bool BinaryTree<Value>::insert(int key, Value* value, bool overwrite)
{
    BinaryNode<Value>* current = m_root;

    int insert_depth = 0;

    // Loop exits instantly if tree is empty
    while(current) {
        // Loop exit condition 1: matching key.
        if(current->m_key == key) {
            if(overwrite) {
                delete current->m_value;
                current->m_value = value;
                return true;
            }
            else {
                // Prevent memory leaks.
                delete value;

                return false;
            }
        }
        else {
            if(current->m_key > key) {
                // Node exists, move down a layer in the tree
                if(current->m_left) {
                    current = current->m_left;
                    insert_depth ++;
                }
                // Node does not exist. Found insertion point.
                else {
                    auto newNode = new BinaryNode<Value>();
                    newNode->m_key = key;
                    newNode->m_value = value;
                    current->m_left = newNode;
                    m_node_depth = max(m_node_depth, insert_depth);
                    m_node_count ++;
                    return true;
                }
            }
            else {
                // Node exists, move down a layer in the tree
                if(current->m_right) {
                    current = current->m_right;
                    insert_depth ++;
                }
                // Node does not exist. Found insertion point.
                else {
                    auto newNode = new BinaryNode<Value>();
                    newNode->m_key = key;
                    newNode->m_value = value;
                    current->m_right = newNode;
                    m_node_depth = max(m_node_depth, insert_depth);
                    m_node_count ++;
                    return true;
                }
            }
        }
    }

    // If we get here, then the tree is empty. Insert new key value pair as root node.
    auto newNode = new BinaryNode<Value>();
    newNode->m_key = key;
    newNode->m_value = value;
    m_root = newNode;
    m_node_depth = 1;
    m_node_count = 1;

    return true;
}


template<class Value>
bool BinaryTree<Value>::remove(int key)
{
    BinaryNode<Value>* current = m_root;
    // Used for overwriting the parent node to point to new children nodes when deleting.
    BinaryNode<Value>** parent_ptr = &m_root;

    // Used for when a node has two children.
    BinaryNode<Value>* shift_node = nullptr;
    BinaryNode<Value>** shift_node_parent = nullptr;

    while(current) {
        if(current->m_key == key) {
            // Found node to delete!
            // First, rearrange the tree so that the node being deleted is no longer used.

            // Determine case
            if(current->m_left)
            {
                if(current->m_right)
                {
                    // The most complex case, two children.

                    // Step one: determine the right-most node in the left subtree.
                    // This node will be the key node during deletion.
                    shift_node_parent = &(current->m_left);
                    shift_node = current->m_left;
                    while(shift_node->m_right) {
                        shift_node_parent = &(shift_node->m_right);
                        shift_node = shift_node->m_right;
                    }

                    // Step 2, overwrite the shift node's parent to hold the shift node's left subtree.
                    *shift_node_parent = shift_node->m_left;
                    
                    // Step 3, move the shift node to the location of the node currently
                    // selected for deletion.
                    shift_node->m_left = current->m_left;
                    shift_node->m_right = current->m_right;
                    *parent_ptr = shift_node;
                }
                else {
                    // Single node becomes new "root" node for the subtree.
                    *parent_ptr = current->m_left;
                }
            }
            else {
                if(current->m_right)
                {
                    // Single node becomes new "root" node for the subtree.
                    *parent_ptr = current->m_right;
                }
                else {
                    // Else, no children, no changes to the tree needed. Remove parent reference.
                    *parent_ptr = nullptr;
                }
            }

            // Delete the current node now that the tree has been rearanged to not use it
            // Set current node pointers to null so we don't accidentally delete any subtrees.
            current->clear();
            // Current node is now safe for deletion.
            delete current;

            m_node_count --;

            // Deletion was successful. Return true.
            return true;
        }
        else {
            if(current->m_key > key)
            {
                parent_ptr = &(current->m_left);
                current = current->m_left;
            }
            else
            {
                parent_ptr = &(current->m_right);
                current = current->m_right;
            }
        }
    }

    // If we reach this point, deletion has failed and the node does not exist. Return false.
    return false;
}

// Search functions.
template<class Value>
bool BinaryTree<Value>::search(int& out, const Value* value) const
{
    // Recursively search the subtrees using a method in the node.
    if(m_root) {
        return m_root->search(out, value);
    }
    return false;
}
template<class Value>
bool BinaryNode<Value>::search(int& out, const Value* value) const
{
    // Search value found at current key.
    if(*m_value == *value) {
        out = m_key;
        return true;
    }
    
    // Check left subtree
    if(m_left) {
        if(m_left->search(out, value))
            return true;
    }

    // Check right subtree.
    if(m_right) {
        if(m_right->search(out, value))
            return true;
    }

    return false;
}

template<class Value>
void _print_binary_node(std::ostream &out, BinaryNode<Value>* node)
{
    if(node) {
        out << node->m_key << '(';
        _print_binary_node(out, node->m_left);
        out << ',';
        _print_binary_node(out, node->m_right);
        out << ')';
    }
}

// Outputs the tree.
template<class Value>
void BinaryTree<Value>::show(std::ostream &out) const
{
    _print_binary_node(out, m_root);
    out << endl;
}

template<class Value>
BinaryTree<Value>::BinaryTree(const BinaryTree<Value>& source)
{
    if(source.m_root)
        m_root = new BinaryNode<Value>(*source.m_root);
    else
        m_root = nullptr;
    m_node_depth = source.m_node_depth;
    m_node_count = source.m_node_count;
}
template<class Value>
BinaryTree<Value>& BinaryTree<Value>::operator=(const BinaryTree<Value>& source)
{
    delete m_root;
    if(source.m_root)
        m_root = new BinaryNode<Value>(*source.m_root);
    else
        m_root = nullptr;
    m_node_depth = source.m_node_depth;
    m_node_count = source.m_node_count;
    return *this;
}

template<class Value>
void BinaryTree<Value>::clear()
{
    delete m_root;
    m_root = nullptr;
    m_node_depth = 0;
    m_node_count = 0;
}
}


