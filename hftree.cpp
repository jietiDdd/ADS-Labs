#include "hftree.h"

hfTree::hfTree(const std::string &text, const Option op)
{
    // TODO: Your code here
    optree = new OpTree;
    if(op == Option::SingleChar){ //构造单字符树
        for(char c : text){ //记录所有字符的频率
            optree->frequecyMap[std::string(1,c)]++;    
        }
    }
    if(op == Option::MultiChar){ //构造组合字符树
        for(int i = 0; i < text.length(); i++){
            if(i + 1 < text.length()){
                std::string multi = text.substr(i,2);
                optree->frequecyMap[multi]++;
            }
        }

        std::string multi1,multi2,multi3;
        int max1,max2,max3;
        max1 = max2 = max3 = INT_MIN;
        for(std::pair<std::string,int> p : optree->frequecyMap){ //找到前三个
            if(p.second > max1 || (p.second == max1 && p.first < multi1)){
                max3 = max2;
                max2 = max1;
                max1 = p.second;
                multi3 = multi2;
                multi2 = multi1;
                multi1 = p.first;
            }
            else if(p.second > max2 || (p.second == max2 && p.first < multi2)){
                max3 = max2;
                max2 = p.second;
                multi3 = multi2;
                multi2 = p.first;
            }
            else if(p.second > max3 || (p.second == max3 && p.first < multi3)){
                max3 = p.second;
                multi3 = p.first;
            }
        }

        optree->frequecyMap.clear(); //清空树，重新遍历

        for(int i = 0; i < text.length(); i++){
            if(i + 1 < text.length()){
                std::string cc = text.substr(i,2);
                if(cc == multi1 || cc == multi2 || cc == multi3){
                    optree->frequecyMap[cc]++;
                    i++;
                    continue;
                }
            }
            std::string c = text.substr(i,1);
            optree->frequecyMap[c]++;
        }

    }

    int size = optree->frequecyMap.size();
    int i = size;
    optree->Length = 2 * size;
    optree->Tree = new Node[optree->Length];

    for(std::pair<std::string,int> p : optree->frequecyMap){
        optree->Tree[i].data = optree->Tree[i].dictionary = p.first;
        optree->Tree[i].weight = p.second;
        optree->Tree[i].parent = optree->Tree[i].left = optree->Tree[i].right = 0;
        i++;
    }

    int min1,min2;
    int x,y;

    for(i = size - 1;i > 0;i--){
        min1 = min2 = INT_MAX;
        x = y = 0;

        for(int j = i + 1; j < optree->Length; j++){
            if(optree->Tree[j].parent == 0){
                if(optree->Tree[j].weight < min1 || (optree->Tree[j].weight == min1 && (optree->Tree[j].dictionary < optree->Tree[y].dictionary))){ //最小
                    min2 = min1;
                    min1 = optree->Tree[j].weight;
                    x = y;
                    y = j;
                }
                else if(optree->Tree[j].weight < min2 || (optree->Tree[j].weight == min1 && (optree->Tree[j].dictionary < optree->Tree[x].dictionary))){ //次小
                    min2 = optree->Tree[j].weight;
                    x = j;
                }
            }
            if(min1 == min2) //权值相等，比较字典序
                if(optree->Tree[y].dictionary > optree->Tree[x].dictionary)
                    std::swap(x,y);
        }

        optree->Tree[i].weight = min1 + min2;
        optree->Tree[i].left = y;
        optree->Tree[i].right = x;
        optree->Tree[i].parent = 0;
        optree->Tree[i].dictionary = (optree->Tree[y].dictionary > optree->Tree[x].dictionary) ? optree->Tree[x].dictionary : optree->Tree[y].dictionary;
        optree->Tree[y].parent = optree->Tree[x].parent = i;
    }
}

std::map<std::string, std::string> hfTree::getCodingTable()
{
    // TODO: Your code here
    for(int i = optree->frequecyMap.size();i < optree->Length; i++){
        std::string code = "";
        int parent = optree->Tree[i].parent;
        int now = i;

        while(parent){
            if(optree->Tree[parent].left == now){ //左子树
                code = '0' + code;
            }
            else{ //右子树
                code = '1' + code;
            }
            now = parent;
            parent = optree->Tree[parent].parent;
        }
        hfCode.insert(std::make_pair(optree->Tree[i].data,code));
    }

    return hfCode;
}