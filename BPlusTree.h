#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "BaseTree.h"
#include <assert.h>
#include <iostream>

using std::cout;
using std::endl;

namespace b_tree {

const int DEFAULT_BPLUS_NODE_SIZE = 5;

template <class Value>
struct BPlusNode {
    BPlusNode<Value>* m_parent;

    void* m_children;

    int* m_keys;

    int m_max_size;
    int m_min_size;
    int m_size;

    bool m_is_leaf;

    BPlusNode() { m_children = nullptr; m_keys = nullptr; m_parent = nullptr; }
    BPlusNode(const BPlusNode<Value>& source)
    {
        m_parent = nullptr;
        init(source.m_is_leaf, source.m_max_size);
        m_size = source.m_size;
        for(int i = 0; i < m_size; i++) {
            m_keys[i] = source.m_keys[i];
        }
        if(m_is_leaf) {
            static_cast<Value**>(m_children)[0] = nullptr;
            for(int i = 1; i < m_size + 1; i ++) {
                auto temp = static_cast<Value**>(source.m_children)[i];
                if(temp)
                {
                    static_cast<Value**>(m_children)[i] = new Value(*temp);
                }
                else
                    static_cast<Value**>(m_children)[i] = nullptr;
            }
        }
        else {
            for(int i = 0; i < m_size + 1; i ++) {
                auto temp = static_cast<BPlusNode<Value>**>(source.m_children)[i];
                assert(temp);
                static_cast<BPlusNode<Value>**>(m_children)[i] = new BPlusNode<Value>(*temp);
                static_cast<BPlusNode<Value>**>(m_children)[i]->m_parent = this;
            }
        }
    }
    ~BPlusNode() {
        if(m_is_leaf)
        {
            Value** children = static_cast<Value**>(m_children);
            for(int i = 1; i < m_size; ++i)
                delete children[i];
            delete[] children;
        }
        else {
            
            auto children = static_cast<BPlusNode<Value>**>(m_children);
            for(int i = 0; i < m_size; ++i)
                delete children[i];
            delete[] children;
        }
        delete[] m_keys;
    }

    BPlusNode<Value>* compute_right_node();
    BPlusNode<Value>* left();

    BPlusNode<Value>*& next() { return static_cast<BPlusNode<Value>**>(m_children)[0]; }

    void init(bool is_leaf, int size)
    {
        m_is_leaf = is_leaf;
        m_max_size = size;
        m_min_size = size / 2;
        m_size = 0;

        m_keys = new int[size + 1];
        if(is_leaf)
        {
            Value** vals = new Value*[size + 2];
            vals[0] = nullptr;
            m_children = vals;
        }
        else {
            m_children = new BPlusNode<Value>*[size + 2];
        }
    }

    int min_key();

    Value*& get(int index) const;

    BPlusNode<Value>* find(int& index, int key);
    const BPlusNode<Value>* find(int& index, int key) const;

    void insert_node(int key, BPlusNode<Value>* value);
    void insert(int key, Value* value);

    // Returns new root node if root node changes
    BPlusNode<Value>* branch_split();
};

template<class Value>
class BPlusTree : public Tree<Value>
{
    private:
        BPlusNode<Value>* m_root;
        int m_node_size;
        int m_node_count;
        int m_depth;

    public:
        BPlusTree() { m_depth = 0; m_node_count = 0; m_root = nullptr; m_node_size = DEFAULT_BPLUS_NODE_SIZE; }
        BPlusTree(int node_size) { m_depth = 0; m_node_count = 0; m_root = nullptr; m_node_size = node_size; }
        BPlusTree(const BPlusTree<Value>& tree);
        ~BPlusTree() { delete m_root; }

        BPlusTree<Value>& operator=(const BPlusTree<Value>& src);

        bool insert(int key, Value *value, bool overwrite = false) override;

        bool remove(int key) override;

        Value* get(int key) const override;

        bool search(int& out, const Value* value) const override;

        int count() const override { return m_node_count; };

        int depth() const override { return m_depth; };

        void show(std::ostream &out) const override;

        void clear() override;
};

}

#include "BPlusTree.hpp"

#endif