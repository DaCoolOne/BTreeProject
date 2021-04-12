

#include <algorithm>
#include <iostream>

// Used for type detection
#include "AvlTree.h"

using std::max;

namespace b_tree {

template<class Value>
AvlNode<Value>* AvlNode<Value>::get(int key)
{
    if (key == m_key) {
        return this;
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
void AvlNode<Value>::recalculate_balance()
{
    m_left_depth = m_left ? max(m_left->m_left_depth, m_left->m_right_depth) + 1 : 0;
    m_right_depth = m_right ? max(m_right->m_left_depth, m_right->m_right_depth) + 1 : 0;
}

template<class Value>
void AvlNode<Value>::shrink_left_depth() {
    if(m_left)
    {
        m_left->shrink_left_depth();
        if(m_left->m_left_depth + 1 < m_left_depth)
        {
            m_left->shrink_right_depth();
            m_left_depth = max(m_left->m_left_depth, m_left->m_right_depth);
        }
    }
    else
    {
        m_left_depth = 0;
    }
}

template<class Value>
void AvlNode<Value>::shrink_right_depth() {
    if(m_right)
    {
        m_right->shrink_left_depth();
        if(m_right->m_left_depth + 1 < m_right_depth)
        {
            m_right->shrink_right_depth();
            m_right_depth = max(m_right->m_left_depth, m_right->m_right_depth);
        }
    }
    else
    {
        m_right_depth = 0;
    }
}

template<class Value>
short AvlNode<Value>::get_successor(AvlNode<Value>*& node) const
{
    if(m_left) {
        // Most complicated case
        if(m_right) {
            // Remove from larger side of tree.
            if(m_left_depth >= m_right_depth) {
                node = m_left;
                while(node->m_right) node = node->m_right;
                return 2;
            }
            // Select right
            else {
                node = m_right;
                while(node->m_left) node = node->m_left;
                return 2;
            }
        }
        else {
            node = m_left;
            return 1;
        }
    }
    else if(m_right) {
        node = m_right;
        return 1;
    }
    node = nullptr;
    return 0;
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

template<class Value>
void AvlNode<Value>::balance(AvlNode<Value>*& root)
{
    // cout << "BALANCE START" << endl;
    AvlNode<Value>* node = this;
    AvlNode<Value>* last = node;

    short insert_depth = max(m_left_depth, m_right_depth);
    short balance;
    short rotation_counter = 0;

    bool ins1, ins2;
    while(node) {
        // cout << "Node " << node->m_key << endl;
        insert_depth++;
        
        node->recalculate_balance();

        balance = node->m_right_depth - node->m_left_depth;
        
        if (balance == 2 || balance == -2) {
            // cout << "IMBALANCE" << balance << endl;

            // cout << "CALC ROT 1 " << node->m_left_depth << "," << node->m_right_depth << endl;
            ins1 = node->m_right_depth > node->m_left_depth;

            // cout << "CALC ROT 2" << endl;
            if(ins1) ins2 = node->m_right->m_right_depth > node->m_right->m_left_depth;
            else ins2 = node->m_left->m_right_depth > node->m_left->m_left_depth;

            // cout << "ROTATE" << endl;
            node = node->rotate(ins1, ins2);
            insert_depth --;

            if(!(node->m_parent)) root = node;
            // cout << "BALANCE RESTORED" << endl;
        }
        last = node;
        node = node->m_parent;
    }
}

// Returns pointer to the node that needs to be deleted.
template<class Value>
AvlNode<Value>* AvlNode<Value>::pop(AvlNode<Value>*& root)
{
    // cout << "Popping" << endl;
    short node_count;
    AvlNode<Value>* result;
    AvlNode<Value>* copy_node = nullptr;
    AvlNode<Value>** parent_node_ptr = nullptr;
    
    // Get reference to the parent's pointer to this node.
    if(m_parent)
    {
        if(m_parent->m_left == this)
            parent_node_ptr = &(m_parent->m_left);
        else // This is the left node
            parent_node_ptr = &(m_parent->m_right);
    }
    else {
        parent_node_ptr = &root;
    }

    // Find node to copy.
    node_count = get_successor(copy_node);
    // Cases
    switch(node_count)
    {
        case 0:
            result = this;
            *parent_node_ptr = nullptr;
            break;
        case 1:
            *parent_node_ptr = copy_node;
            copy_node->m_parent = m_parent;
            result = this;
            break;
        case 2:
            // Case of both nodes have children.
            m_key = copy_node->m_key;
            m_value = copy_node->m_value;
            m_value = nullptr;
            result = copy_node->pop(root);
            break;
    }
    return result;
}

// Gets and returns a pointer to the value at the key, or nullptr if
// the key does not exist in the tree.
template<class Value>
Value* AvlTree<Value>::get(int key) const {
    if (m_root)
    {
        AvlNode<Value>* node = m_root->get(key);
        if(node) return node->m_value;
    }
    
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
        m_node_count = 1;
        return true;
    }

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
                m_node_count ++;
                break;
            }
        }
        // Insert right
        else {
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
                m_node_count ++;
                break;
            }
        }
    }

    // Balance the tree propogating back to the root.
    if (node) node->balance(m_root);

    // Insertion complete. Was successful if node exists.
    return static_cast<bool>(node);
}

// Just copy+pasted from BinaryTree.h
// Still need to actually write this code.
template<class Value>
bool AvlTree<Value>::remove(int key)
{
    // If root node does not exist, removal fails.
    if(!m_root) return false;

    // Find node to remove
    AvlNode<Value>* node_to_remove = m_root->get(key);
    AvlNode<Value>* delete_node;
    AvlNode<Value>* temp;
    if(node_to_remove)
    {
        delete_node = (node_to_remove->pop(m_root));
        temp = delete_node->m_parent;

        m_node_count --;

        delete_node->clear();
        delete delete_node;

        // All parts of the tree below the deleted node do not have
        // any changes needed to their balance factor.

        // Go backwards through the tree to root updating balance factors.
        if(temp) temp->balance(m_root);

        // Successful deletion.
        return true;
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
void _print_avl_node(std::ostream &out, AvlNode<Value>* node)
{
    if(node) {
        // out << node->m_key << "[" << node->m_left_depth << node->m_right_depth << "]" << '(' << flush;
        out << node->m_key << '(';
        _print_avl_node(out, node->m_left);
        out << ',';
        _print_avl_node(out, node->m_right);
        out << ')';
    }
}

// Outputs the tree.
template<class Value>
void AvlTree<Value>::show(std::ostream &out) const
{
    _print_avl_node(out, m_root);
    out << endl;
}

}

