#include "compressed_radix_tree.hpp"

CompressedRadixTree::CompressedRadixTree()
{
    // To Be Implemented
    root = new compressed_radix_tree_node();
    sizeNumber = 1;
}

CompressedRadixTree::~CompressedRadixTree()
{
    // To Be Implemented
    del(root);
    sizeNumber = 0;
}

void CompressedRadixTree::insert(int32_t value)
{
    uint32_t unsignedValue = (uint32_t)value;
    // To Be Implemented
    bool isFound;
    uint32_t finishBits = 0;
    // 查找停在哪个节点
    compressed_radix_tree_node *p = findInter(value,isFound,finishBits);
    if(isFound){ // 查找成功，不用插入
        return;
    }
    uint32_t prefix = (p->dataBits == 0) ? 0 : ((unsignedValue << finishBits) >> (32 - p->dataBits));
    uint32_t bits = (unsignedValue >> (30 - finishBits)) & 0x3;
    if(p->data == prefix){ // 前缀能匹配，插入孩子
        finishBits += p->dataBits;
        bits = (unsignedValue >> (30 - finishBits)) & 0x3;
        p->children[bits] = new compressed_radix_tree_node(((unsignedValue << finishBits) >> finishBits),(32 - finishBits),p);
        p->isLeaf = false;
        p->childrenNumber++;
        sizeNumber++;
    }
    else{ // 不能匹配，分裂节点
        uint32_t newPublicPrefix;
        uint32_t recentPublicPrefix;
        uint32_t dataBits = p->dataBits;
        uint32_t recentData = p->data;
        uint32_t newChild;
        uint32_t recentChild;
        compressed_radix_tree_node *tmp;
        for(uint32_t publicBits = 0; publicBits < dataBits; publicBits += 2){ // 保证节点的比特数为2的倍数
            newPublicPrefix = prefix >> (dataBits - publicBits - 2);
            recentPublicPrefix = recentData >> (dataBits - publicBits - 2);
            if(newPublicPrefix == recentPublicPrefix){ // 前缀相同，继续检查
                continue;
            }
            else{ // 前缀不同，说明找到公共前缀，进行分裂
                p->data = prefix >> (dataBits - publicBits);
                p->dataBits = publicBits;
                newChild = unsignedValue << (finishBits + publicBits) >> (finishBits + publicBits);
                recentChild = recentData << (32 - dataBits + publicBits) >> (32 - dataBits + publicBits);
                tmp = new compressed_radix_tree_node(recentChild,dataBits - publicBits,p);
                for(int i = 0; i < 4; i++){
                    tmp->children[i] = p->children[i];
                    if(p->children[i]) p->children[i]->parent = tmp;
                }
                tmp->childrenNumber = p->childrenNumber;
                tmp->isLeaf = (tmp->childrenNumber == 0) ? true : false;
                for(int i = 0; i < 4; i++){
                    p->children[i] = NULL;
                }
                p->children[newPublicPrefix & 0x3] = new compressed_radix_tree_node(newChild,32-finishBits-publicBits,p);
                p->children[recentPublicPrefix & 0x3] = tmp;
                sizeNumber += 2;
                p->childrenNumber = 2;
                p->isLeaf = false;
                break;
            }
        }
    }
}

void CompressedRadixTree::remove(int32_t value)
{
    // To Be Implemented
    bool isFound;
    uint32_t finishBits = 0;
    // 查找删除节点位置
    compressed_radix_tree_node* p = findInter(value,isFound,finishBits);
    compressed_radix_tree_node* tmp;

    if(isFound == false) return;

    while(p != root){ // 删除直到根节点
        tmp = p;
        if(p->childrenNumber == 0){
            p = p->parent;
            p->childrenNumber--;
            sizeNumber--;
            for(int i = 0; i < 4; i++){
                if(p->children[i] == tmp){
                    p->children[i] = NULL;
                    break;
                }
            }
            delete tmp;
        }
        else if(p->childrenNumber == 1){
            for(int i = 0;i < 4; i++){
                if(p->children[i]){ // 找到要合并的孩子
                    tmp = p->children[i];
                    p->data = (p->data << tmp->dataBits) | tmp->data;
                    p->dataBits += tmp->dataBits;
                    for(int i = 0; i < 4; i++){
                        p->children[i] = tmp->children[i];
                        if(tmp->children[i]) tmp->children[i]->parent = p;
                    }
                    p->childrenNumber = tmp->childrenNumber;
                    p->isLeaf = (p->childrenNumber == 0) ? true : false;
                    delete tmp;
                    sizeNumber--;
                }   
            }
            return;
        }
        else return;
    }
}

bool CompressedRadixTree::find(int32_t value)
{
    // To Be Implemented
    bool isFound;
    uint32_t finishBits = 0;
    findInter(value,isFound,finishBits);
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
    return heightInter(root);
}

void CompressedRadixTree::del(compressed_radix_tree_node* node)
{
    if(node == NULL) return;
    for(int i = 0; i < 4; i++){
        del(node->children[i]);
    }
    delete node;
}

compressed_radix_tree_node * CompressedRadixTree::findInter(uint32_t value,bool &isFound,uint32_t &finishBits)
{
    compressed_radix_tree_node *p = root; // 从根结点出发
    uint32_t bits; // 孩子的索引
    uint32_t prefix; // 记录前缀
    finishBits = 0;

    uint32_t unsignedValue = (uint32_t)value;

    while(finishBits != 32){ // 当尚未检查完所有位时
        bits = (unsignedValue >> (30 - finishBits)) & 0x3;
        if(p->children[bits]){ // 该前缀对应的节点非空
            p = p->children[bits];
            prefix = (unsignedValue << finishBits) >> (32 - p->dataBits);
            if(prefix == p->data){ // 节点值对上
                finishBits += p->dataBits;
            }
            else{ // 没对上，即查找失败
                isFound = false;
                return p;
            }
        }
        else{ // 没有对应的孩子，也是查找失败
            finishBits -= p->dataBits;
            isFound = false;
            return p;
        }
    }

    isFound = true;
    return p;
}

uint32_t CompressedRadixTree::heightInter(compressed_radix_tree_node *p)
{
    if(p == NULL) return 0; // 空结点
    if(p->isLeaf) return 1; // 叶子节点，退栈
    uint32_t maxHeight = 0;
    uint32_t newHeight = 0;
    for(int i = 0; i < 4; i++){ // 递归检查所有子树，找到子树中的最高高度
        newHeight = heightInter(p->children[i]);
        if(newHeight > maxHeight){
            maxHeight = newHeight;
        }
    }
    return maxHeight + 1;
}
