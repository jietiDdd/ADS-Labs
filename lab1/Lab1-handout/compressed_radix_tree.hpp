// implement a radix tree that support node compressino and store int32_t values
// each parent node has 4 children, representing 2 bits
#include <cstdint>
#include <algorithm>
#include <vector>
#include "tree.hpp"

// 基数树的节点结构体
struct compressed_radix_tree_node
{
     /* data */
    uint32_t data; // 存放该节点的数值
    uint32_t dataBits; // 记录压缩节点的比特数
    bool isLeaf; // 是否是叶子节点
    std::vector<compressed_radix_tree_node*> children; // 存放孩子节点
    uint32_t childrenNumber; // 孩子数目
    compressed_radix_tree_node *parent; // 记录父结点

    compressed_radix_tree_node(uint32_t data,uint32_t dataBits,compressed_radix_tree_node *parent):data(data),dataBits(dataBits),isLeaf(true),children(4,NULL),childrenNumber(0),parent(parent){} // 初始时四个孩子都是NULL
    compressed_radix_tree_node():children(4,NULL),childrenNumber(0),parent(NULL),data(0),dataBits(0),isLeaf(true){} // 只为根结点服务
};

class CompressedRadixTree : public Tree
{
    // To Be Implemented
    compressed_radix_tree_node* root; // 根结点
    uint32_t sizeNumber; // 节点个数

    void del(compressed_radix_tree_node* node); // 内置的递归删除节点函数

    compressed_radix_tree_node *findInter(uint32_t value,bool &isFound,uint32_t &finishBits); // 内置的查找函数，finishBits记录已经检查了的位数
    uint32_t heightInter(compressed_radix_tree_node *p);

public:
    CompressedRadixTree();
    ~CompressedRadixTree();
    // basic operation
    void insert(int32_t value);
    void remove(int32_t value);
    bool find(int32_t value);
    // statistics
    uint32_t size();
    uint32_t height();
};