//
// Created by lenovo on 2024/4/22.
//

#ifndef LAB1_HANDOUT_RB_TREE_SET_H
#define LAB1_HANDOUT_RB_TREE_SET_H

#include "tree.hpp"
#include <set>
#include <cstdint>

class RedBlackTree : public Tree
{
    std::set<int32_t> rb_tree;
public:
    void insert(int32_t value){
        rb_tree.insert(value);
    }
    void remove(int32_t value){
        rb_tree.erase(value);
    }
    bool find(int32_t value){
        auto it = rb_tree.find(value);
        return it != 
    }
};


#endif //LAB1_HANDOUT_RB_TREE_SET_H
