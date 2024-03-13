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
        int weight;
        int parent,left,right;
        std::string dictionary; //存放字典序
    };

    struct OpTree{
        Node *Tree;
        int Length;
        std::map<std::string,int> frequecyMap; //记录出现频率
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
};

#endif