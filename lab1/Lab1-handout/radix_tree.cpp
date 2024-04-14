#include "radix_tree.hpp"

RadixTree::RadixTree()
{
    // To Be Implemented
    root = new radix_tree_node();
    sizeNumber = 1;
    heightNumber = 1;
}

RadixTree::~RadixTree()
{
    // To Be Implemented
    del(root);
    sizeNumber = 0;
    heightNumber = 0;
}

void RadixTree::insert(int32_t value)
{
    // To Be Implemented
    radix_tree_node *p = root; // 从根结点出发
    int32_t bits; // 存放高位的两个比特
    heightNumber = 17;

    for(int i = 0; i < 16; i++){
        bits = (value >> (30 - 2 * i)) & 0x3; // 读取高位的两个比特
        if(p->children[bits] == NULL){ // 未创建，申请孩子节点
            p->childrenNumber++;
            p->children[bits] = new radix_tree_node(bits,p);
            sizeNumber++;
        }
        p = p->children[bits]; // 插入到对应孩子节点的子树
        if(i == 15) p->isLeaf = true;
    }
}

void RadixTree::remove(int32_t value)
{
    // To Be Implemented
    bool isFound;
    radix_tree_node *p = findInter(value,isFound); // 获取叶子节点
    radix_tree_node *tmp;

    if(isFound == false) return; // 节点不存在，不用删除

    while(p != root){ // 删除直到根结点
        tmp = p;
        if(p->childrenNumber == 0){ // 没有孩子，删除
            p = p->parent;
            delete tmp;
        }
        else return;
    }
}

bool RadixTree::find(int32_t value)
{
    // To Be Implemented
    bool isFound;
    findInter(value,isFound);
    return isFound;
}

uint32_t RadixTree::size()
{
    // To Be Implemented
    return sizeNumber;
}

uint32_t RadixTree::height()
{
    // To Be Implemented
    return heightNumber;
}

void RadixTree::del(radix_tree_node* node) // 内置的递归删除节点函数
{
    if(node == NULL) return;
    for(int i = 0; i < 4; i++){
        del(node->children[i]);
    }
    delete node;
}

radix_tree_node* RadixTree::findInter(int32_t value,bool &isFound)
{
    radix_tree_node *p = root; // 从根结点出发
    int32_t bits; // 存放高位的两个比特

    for(int i = 0; i < 16; i++){ // 共32位
        bits = (value >> (30 - 2 * i)) & 0x3;
        if(p->children[bits]){ // 该前缀对应的节点非空
            p = p->children[bits];
        }
        else{ // 查找失败
            isFound = false;
            return NULL;
        }
    }

    isFound = true;
    return p;
}