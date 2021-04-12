

#include "BTree.h"
#include <algorithm>

using std::swap;

namespace b_tree {

template<class Value>
Value* BNode<Value>::getValueAtKey(int key) const
{
    const BNode<Value>* node_ptr = nullptr;
    int index = getNodeIndex(node_ptr, key);
    if (index != -1) return node_ptr->m_values[index];
    return nullptr;
}

template<class Value>
bool BNode<Value>::search(int& out, const Value* val) const
{
    for(int i = 0; i <= m_size; i ++) {
        if(m_children[i])
            if(m_children[i]->search(out, val))
                return true;
    }

    for(int i = 0; i < m_size; i ++) {
        if(*m_values[i] == *val)
        {
            out = m_keys[i];
            return true;
        }
    }

    return false;
}

template<class Value>
int BNode<Value>::getNodeIndex(BNode<Value>*& out, int key)
{
    // Linear search
    // Todo: make this a binary search?
    for(int i = 0; i < m_size; i ++) {
        if(m_keys[i] == key) {
            out = this;
            return i;
        }
        else if(key < m_keys[i]) {
            if(m_children[i]) {
                return m_children[i]->getNodeIndex(out, key);
            }
            else {
                out = this;
                return -1;
            }
        }
    }

    if(m_children[m_size]) return m_children[m_size]->getNodeIndex(out, key);
    
    out = this;
    return -1;
}

template<class Value>
int BNode<Value>::getNodeIndex(const BNode<Value>*& out, int key) const
{
    // Linear search
    // Todo: make this a binary search?
    for(int i = 0; i < m_size; i ++) {
        if(m_keys[i] == key) {
            out = this;
            return i;
        }
        else if(key < m_keys[i]) {
            if(m_children[i]) {
                return m_children[i]->getNodeIndex(out, key);
            }
            else {
                out = this;
                return -1;
            }
        }
    }

    if(m_children[m_size]) {
        return m_children[m_size]->getNodeIndex(out, key);
    }
    else {
        out = this;
        return -1;
    }
}

template<class Value>
void BNode<Value>::setLeftChild(BNode<Value>* child)
{
    if(child) child->m_parent = this;
    m_children[0] = child;
}

template<class Value>
BNode<Value>* BNode<Value>::split()
{
    if(m_size > m_max_size) {
        BNode<Value>* right_node = new BNode<Value>();
        right_node->setSize(m_max_size);
        right_node->setLeftChild(m_children[m_min_size + 1]);
        // Build right node
        for(int i = m_min_size + 1; i < m_max_size + 1; i ++) {
            right_node->insert(m_keys[i], m_values[i], m_children[i+1]);
        }
        // Insert middle node into parent.
        m_size = m_min_size;

        if(m_parent) {
            m_parent->insert(m_keys[m_size], m_values[m_size], right_node);
            return m_parent->split();
        }
        // No parent node to insert into, create new root node.
        else {
            m_parent = new BNode<Value>();
            m_parent->setSize(m_max_size);
            m_parent->setLeftChild(this);
            m_parent->insert(m_keys[m_size], m_values[m_size], right_node);
            return m_parent;
        }

    }

    return nullptr;
}

template<class Value>
void BNode<Value>::insert(int new_key, Value* value, BNode<Value>* child)
{
    if(child) child->m_parent = this;

    for(int i = 0; i < m_size+1; i++) {
        // Shifts all elements down and places element into array.
        if(new_key < m_keys[i] || i == m_size) {
            swap(new_key, m_keys[i]);
            swap(value, m_values[i]);
            swap(child, m_children[i+1]);
        }
    }
    // Node has now increased in size by one.
    m_size ++;

    return;
}

template<class Value>
void BNode<Value>::deleteChildren()
{
    for(int i = 0; i < m_size + 1; i ++) {
        if(m_children[i]) {
            m_children[i]->deleteChildren();
            delete m_children[i];
        }
    }
}

template<class Value>
Value* BTree<Value>::get(int key) const
{
    if(m_root) return m_root->getValueAtKey(key);

    return nullptr;
}

template<class Value>
bool BTree<Value>::search(int& out, const Value* value) const
{
    if(m_root)
        return m_root->search(out, value);
    
    return false;
}

template<class Value>
bool BTree<Value>::insert(int key, Value* value, bool overwrite)
{
    if(!m_root)
    {
        m_root = new BNode<Value>();
        m_root->setSize(m_node_default);
        m_root->m_children[0] = nullptr;
        m_root->insert(key, value, nullptr);
        m_node_count = 1;
        return true;
    }

    BNode<Value>* insert_ptr;
    int index = m_root->getNodeIndex(insert_ptr, key);

    // Index already exists
    if(index != -1) {

        // Value is full
        if(insert_ptr->m_values[index])
        {
            // Overwrite
            if(overwrite) {
                delete insert_ptr->m_values[index];
                insert_ptr->m_values[index] = value;
            }
            return overwrite;
        }
        // Value has previously existed but has since been deleted
        else {
            insert_ptr->m_values[index] = value;
            m_node_count ++;
            return true;
        }
    }

    // Index does not already exist.
    insert_ptr->insert(key, value, nullptr);
    m_node_count ++;
    BNode<Value>* new_root = insert_ptr->split();

    if(new_root)
    {
        m_root = new_root;
        m_node_depth ++;
    }

    return true;
}

template<class Value>
void _print_b_node(std::ostream &out, BNode<Value>* node)
{
    // Don't print nullptrs
    if(!node) return;

    for(int i = 0; i < node->m_size; i ++) {
        out << node->m_keys[i];
        if(i < node->m_size - 1)
            out << "|";
    }
    out << '(';
    for(int i = 0; i < node->m_size + 1; i ++) {
        _print_b_node(out, node->m_children[i]);
        if(i < node->m_size)
            out << ",";
    }
    out << ')';
}

template<class Value>
void BTree<Value>::show(std::ostream &out) const
{
    _print_b_node(out, m_root);
}

template<class Value>
bool BTree<Value>::remove(int key)
{
    if(!m_root) return false;
    BNode<Value>* temp;
    int index = m_root->getNodeIndex(temp, key);
    if(index == -1) return false;
    if(temp->m_values[index])
    {
        delete temp->m_values[index];
        temp->m_values[index] = nullptr;
        m_node_count --;
        return true;
    }
    else return false;
}

}

