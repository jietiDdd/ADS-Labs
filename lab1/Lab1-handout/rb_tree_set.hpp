#ifndef LAB1_HANDOUT_RB_TREE_SET_H
#define LAB1_HANDOUT_RB_TREE_SET_H

#include "tree.hpp"
#include <set>
#include <cstdint>


// 基于std::set的红黑树
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
        return it != rb_tree.end();
    }
    uint32_t size(){
        return 0;
    }
    uint32_t height(){return 0;}
};


#endif //LAB1_HANDOUT_RB_TREE_SET_H
