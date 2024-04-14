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
    int32_t data; // 存放该节点的数值
    int32_t dataBits; // 记录压缩节点的比特数
    bool isLeaf; // 是否是叶子节点
    std::vector<compressed_radix_tree_node*> children; // 存放孩子节点
    int32_t childrenNumber; // 孩子数目
    compressed_radix_tree_node *parent; // 记录父结点

    compressed_radix_tree_node(int32_t data,int32_t dataBits,compressed_radix_tree_node *parent):data(data),dataBits(dataBits),isLeaf(false),children(4,NULL),childrenNumber(0),parent(parent){} // 初始时四个孩子都是NULL
    compressed_radix_tree_node():children(4,NULL),childrenNumber(0),parent(NULL){} // 只为根结点服务
};

class CompressedRadixTree : public Tree
{
    // To Be Implemented
    compressed_radix_tree_node* root; // 根结点
    int32_t sizeNumber; // 节点个数
    int32_t heightNumber; // 树的高度

    void del(compressed_radix_tree_node* node); // 内置的递归删除节点函数

    compressed_radix_tree_node *findInter(int32_t value,bool &isFound); // 内置的查找函数，需要叶子节点

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