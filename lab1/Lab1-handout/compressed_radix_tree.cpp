#include "compressed_radix_tree.hpp"

CompressedRadixTree::CompressedRadixTree()
{
    // To Be Implemented
    root = new compressed_radix_tree_node();
    sizeNumber = 1;
    heightNumber = 1;
}

CompressedRadixTree::~CompressedRadixTree()
{
    // To Be Implemented
    del(root);
    sizeNumber = 0;
    heightNumber = 0;
}

void CompressedRadixTree::insert(int32_t value)
{
    // To Be Implemented
}

void CompressedRadixTree::remove(int32_t value)
{
    // To Be Implemented
}

bool CompressedRadixTree::find(int32_t value)
{
    // To Be Implemented
    bool isFound;
    findInter(value,isFound);
    return isFound;
}

uint32_t CompressedRadixTree::size()
{
    // To Be Implemented
    return sizeNumber;
}

uint32_t CompressedRadixTree::height()
{
    // To Be Implemented
    return heightNumber;
}

void CompressedRadixTree::del(compressed_radix_tree_node* node)
{
    if(node == NULL) return;
    for(int i = 0; i < 4; i++){
        del(node->children[i]);
    }
    delete node;
}

compressed_radix_tree_node * CompressedRadixTree::findInter(int32_t value,bool &isFound)
{
    compressed_radix_tree_node *p = root; // 从根结点出发
    int32_t bits; // 孩子的索引
    int32_t finishBits = 0; // 记录已经检查的位数
    int32_t prefix; // 记录前缀

    while(finishBits != 32){ // 当尚未检查完所有位时
        bits = (value >> (30 - finishBits)) & 0x3;
        if(p->children[bits]){ // 该前缀对应的节点非空
            p = p->children[bits];
            prefix = (value << finishBits) >> (32 - p->dataBits);
            if(prefix == p->data){ // 节点值对上
                finishBits += p->dataBits;
            }
            else{ // 没对上，即查找失败
                isFound = false;
                return p;
            }
        }
        else{ // 没有对应的孩子，也是查找失败
            isFound = false;
            return p;
        }
    }

    isFound = true;
    return p;
}