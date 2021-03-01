

#include <algorithm>
#include <iostream>

// Used for type detection
#include "AvlTree.h"

using std::cout;
using std::endl;
using std::max;

namespace b_tree {

template<class Value>
Value* AvlNode<Value>::get(int key) const
{
    if (key == m_key) {
        return m_value;
    }
    else if (key < m_key) {
        if (m_left)
            return m_left->get(key);
        else
            return nullptr;
    }
    else {
        if (m_right)
            return m_right->get(key);
        else
            return nullptr;
    }
}

template<class Value>
bool AvlNode<Value>::search(int& out, const Value* value) const
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

// Bool true = right, bool false = left.
// r1 = true, r2 = false means a RL rotation
// r1 = false, r2 = false means a LL rotation.
// Returns the new root node.
template<class Value>
AvlNode<Value>* AvlNode<Value>::rotate(bool r1, bool r2)
{
    // Cases
    if(r1) {
        // Compute the new root node.
        AvlNode<Value>* new_root;
        if(r2) {
            new_root = m_right;
        }
        else {
            new_root = m_right->m_left;
        }
            
        // Update parent node if this node is not root.
        if(m_parent) {
            // We are parent's left node.
            if (m_parent->m_left == this)
                m_parent->m_left = new_root;
            // We are parent's right node.
            else
                m_parent->m_right = new_root;
        }
        new_root->m_parent = m_parent;

        // RR rotation
        if(r2) {
            // Perform the rotation.
            m_right = new_root->m_left;
            m_right_depth = new_root->m_left_depth;
            
            if(m_right) { m_right->m_parent = this; }

            new_root->m_left = this;
            new_root->m_left_depth = max(m_right_depth, m_left_depth) + 1;

            m_parent = new_root;

            return new_root;
        }
        // RL rotation
        else {
            // Perform the rotation.
            if(new_root->m_right) { new_root->m_right->m_parent = m_right; }
            if(new_root->m_left) { new_root->m_left->m_parent = this; }
            
            m_right->m_left = new_root->m_right;
            m_right->m_left_depth = new_root->m_right_depth;

            m_right->m_parent = new_root;
            new_root->m_right = m_right;

            m_right = new_root->m_left;
            m_right_depth = new_root->m_left_depth;

            new_root->m_left = this;

            m_parent = new_root;
            
            new_root->m_right_depth = max(
                new_root->m_right->m_left_depth, new_root->m_right->m_right_depth) + 1;
            
            new_root->m_left_depth = max(
                new_root->m_left->m_left_depth, new_root->m_left->m_right_depth) + 1;
            
            return new_root;
        }
    }
    else {
        // Compute the new root node.
        AvlNode<Value>* new_root = m_left;
        if(r2) {
            new_root = m_left->m_right;
        }
        else {
            new_root = m_left;
        }

        // Update parent node if this node is not root.
        if(m_parent) {
            
            // We are parent's left node.
            if (m_parent->m_left == this)
                m_parent->m_left = new_root;
            // We are parent's right node.
            else
                m_parent->m_right = new_root;
        }
        new_root->m_parent = m_parent;
        
        // LR rotation
        if(r2) {
            
            // Perform the rotation.
            if(new_root->m_left) { new_root->m_left->m_parent = m_left; }
            if(new_root->m_right) { new_root->m_right->m_parent = this; }

            m_left->m_right = new_root->m_left;
            m_left->m_right_depth = new_root->m_left_depth;

            m_left->m_parent = new_root;
            new_root->m_left = m_left;

            m_left = new_root->m_right;
            m_left_depth = new_root->m_right_depth;

            new_root->m_right = this;

            m_parent = new_root;
            
            new_root->m_right_depth = max(
                new_root->m_right->m_left_depth, new_root->m_right->m_right_depth) + 1;
            
            new_root->m_left_depth = max(
                new_root->m_left->m_left_depth, new_root->m_left->m_right_depth) + 1;
            
            return new_root;
        }
        // LL rotation
        else {
            // Perform the rotation.
            m_left = new_root->m_right;
            m_left_depth = new_root->m_right_depth;

            if(m_left) { m_left->m_parent = this; }

            new_root->m_right = this;
            new_root->m_right_depth = max(m_right_depth, m_left_depth) + 1;

            m_parent = new_root;

            return new_root;
        }
    }
}

// Gets and returns a pointer to the value at the key, or nullptr if
// the key does not exist in the tree.
template<class Value>
Value* AvlTree<Value>::get(int key) const {
    if (m_root)
        return m_root->get(key);
    else
        return nullptr;
}

template<class Value>
bool AvlTree<Value>::insert(int key, Value *value, bool overwrite)
{
    // Root node.
    if(!m_root) {
        m_root = new AvlNode<Value>();
        m_root->m_key = key;
        m_root->m_value = value;
        m_node_depth = 1;
        m_node_count = 1;
        return true;
    }

    // true means right, false means left
    int bt_len = 0;
    bool* bt = new bool[m_node_depth + 1];
    bool success = false;

    AvlNode<Value>* node = m_root;

    // Node insertion
    int depth = 0;
    while(node) {
        depth ++;
        if (key == node->m_key) {
            if (overwrite) {
                delete node->m_value;
                node->m_value = value;
                return true;
            }
            else {
                // Again, prevents memory leaks.
                // Maybe we should copy values instead of using pointers?
                delete value;
                return false;
            }
        }
        // Insert left
        else if (key < node->m_key) {
            bt[bt_len++] = false;
            if(node->m_left) {
                node = node->m_left;
            }
            // Found insertion point.
            else {
                // cout << "Insertion found left of " << node->m_key << endl;
                AvlNode<Value>* n = new AvlNode<Value>();
                n->m_parent = node;
                n->m_key = key;
                n->m_value = value;
                node->m_left = n;
                m_node_depth = max(depth, m_node_depth);
                break;
            }
        }
        // Insert right
        else {
            bt[bt_len++] = true;
            if(node->m_right) {
                node = node->m_right;
            }
            // Found insertion point.
            else {
                // cout << "Insertion found right of " << node->m_key << endl;
                AvlNode<Value>* n = new AvlNode<Value>();
                n->m_parent = node;
                n->m_key = key;
                n->m_value = value;
                node->m_right = n;
                m_node_depth = max(depth, m_node_depth);
                break;
            }
        }
    }

    // cout << "Rebalance" << endl;
    // Rebalance the tree.
    short insert_depth;
    short balance;
    short rotation_counter = 0;
    for(int i = bt_len - 1; i >= 0; i --) {
        // cout << i << endl;
        insert_depth = (bt_len - rotation_counter - i);
        // right
        if(bt[i])
            node->m_right_depth = max(node->m_right_depth, insert_depth);
        // left
        else
            node->m_left_depth = max(node->m_left_depth, insert_depth);
        balance = node->m_right_depth - node->m_left_depth;
        // Imbalance found
        if (balance == 2 || balance == -2) {
            // cout << "Rotating" << endl;
            node = node->rotate(bt[i], bt[i+1]);
            rotation_counter ++;
            if(i == 0) {
                m_root = node;
            }
            // cout << "Balance restored (somewhat)" << endl;
        }
        // cout << node->m_key << endl;
        node = node->m_parent;
    }
    // cout << "Insertion complete!" << endl;

    delete[] bt;

    return success;
}

// Just copy+pasted from BinaryTree.h
// Still need to actually write this code.
template<class Value>
bool AvlTree<Value>::remove(int key)
{
    
    AvlNode<Value>* current = m_root;
    // Used for overwriting the parent node to point to new children nodes when deleting.
    AvlNode<Value>** parent_ptr = &m_root;

    // Used for when a node has two children.
    AvlNode<Value>* shift_node = nullptr;
    AvlNode<Value>** shift_node_parent = nullptr;

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
bool AvlTree<Value>::search(int& out, const Value* value) const
{
    // Recursively search the subtrees using a method in the node.
    if(m_root) {
        return m_root->search(out, value);
    }
    return false;
}

template<class Value>
void _print_node(std::ostream &out, AvlNode<Value>* node)
{
    if(node) {
        // out << node->m_key << "[" << node->m_left_depth << node->m_right_depth << "]" << '(';
        out << node->m_key << '(';
        _print_node(out, node->m_left);
        out << ',';
        _print_node(out, node->m_right);
        out << ')';
    }
}

// Outputs the tree.
template<class Value>
void AvlTree<Value>::show(std::ostream &out)
{
    _print_node(out, m_root);
    out << endl;
}

}

