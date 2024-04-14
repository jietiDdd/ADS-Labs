#ifndef HFTREE_H
#define HFTREE_H

#include <string>
#include <map>

class hfTree
{

private:
    // TODO: Your code here
    struct Node{
        std::string data;
        uint64_t weight;
        uint64_t parent,left,right;
        std::string dictionary; //存放字典序
    };

    struct OpTree{
        Node *Tree;
        uint64_t Length;
        std::map<std::string,uint64_t> frequecyMap; //记录出现频率
    };

    OpTree *optree;

    std::map<std::string,std::string> hfCode; //记录编码表
    
public:
    enum class Option
    {
        SingleChar,
        MultiChar
    };
    hfTree(const std::string &text, const Option op);
    std::map<std::string, std::string> getCodingTable();

    ~hfTree(){
        delete [] optree->Tree;
        delete optree;
    }
};

#endif