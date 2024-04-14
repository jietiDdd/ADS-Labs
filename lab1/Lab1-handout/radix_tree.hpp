
#include <cstdint>
#include <vector>
#include "tree.hpp"

// 基数树的节点结构体
struct radix_tree_node
{
     /* data */
    int32_t data; // 存放该节点的数值
    bool isLeaf; // 是否是叶子节点
    std::vector<radix_tree_node*> children; // 存放孩子节点
    int32_t childrenNumber; // 孩子数目
    radix_tree_node *parent; // 记录父结点

    radix_tree_node(int32_t data,radix_tree_node *parent):data(data),isLeaf(false),children(4,NULL),childrenNumber(0),parent(parent){} // 初始时四个孩子都是NULL
    radix_tree_node():children(4,NULL),childrenNumber(0),parent(NULL){} // 只为根结点服务
};

class RadixTree : public Tree
{
    // To Be Implemented
    radix_tree_node* root; // 根结点
    int32_t sizeNumber; // 节点个数
    int32_t heightNumber; // 树的高度

    void del(radix_tree_node* node); // 内置的递归删除节点函数

    radix_tree_node* findInter(int32_t value,bool &isFound); // 内置的查找函数，可以定位到查找时落在哪个叶子节点

public:
    RadixTree();
    ~RadixTree();
    // basic operation
    void insert(int32_t value);
    void remove(int32_t value);
    bool find(int32_t value);
    // statistics
    uint32_t size();
    uint32_t height();
};
