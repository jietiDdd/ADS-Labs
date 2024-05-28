#include <iostream>
#include "base.hpp"
#include <vector>
#include <cstring>

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <random>
#include <cassert>

#include "../util/parameter.hpp"
#include "../util/util.hpp"

namespace HNSWLab {
    
    
    class HNSW : public AlgorithmInterface {
    public:
        // you can add more parameter to initialize HNSW
        int dim; // 向量维度

        int entry; // 入口节点的label

        int maxL; // 入口节点对应的level，也是当前跳表的最高层

        std::unordered_map<int,const int*> vector; // 存储向量的信息

        std::unordered_map<int, std::unordered_map<int, std::unordered_set<int>>> neighbors; // 存储各个level的各个label结点的邻近节点label集合

        HNSW(int dim = 128) {this->dim = dim; this->entry = -1; this->maxL = -1;}

        void insert(const int *item, int label);

        std::vector<int> query(const int *query, int k);

        std::vector<int> concurrent_query(const int *query, int k, int thread_id); // 并发时使用的查询函数

        std::unordered_set<int> search_layer(int label, int ep, int ef, int lc);

        int nearest(int label, std::unordered_set<int> newNeighbors);

        int furthest(int label, std::unordered_set<int> newNeighbors);

        std::unordered_set<int> select_neighbors(int label, std::unordered_set<int> newNeighbors, int m, int lc);

        ~HNSW() {}
    };

    /**
     * input:
     * iterm: the vector to be inserted
     * label: the label(id) of the vector
    */
    void HNSW::insert(const int *item, int label) {
        //TODO
        
        // 1. 计算新结点的插入层级
        vector[label] = item; // 维护向量信息
        std::unordered_set<int> newNeighbors; // 存放当前插入节点的邻近元素集合
        int ep = entry; // 入口节点
        int level = get_random_level();

        // 2. 搜索到L+1
        for(int lc = maxL; lc >= level + 1; lc--){
            std::unordered_set<int> w = search_layer(label, ep, 1, lc);
            newNeighbors.insert(w.begin(), w.end());
            ep = nearest(label, newNeighbors);
        }

        // 3. 从L层向下搜索
        for(int lc = std::min(maxL, level); lc >= 0; lc--){
            newNeighbors = search_layer(label, ep, ef_construction, lc);
            neighbors[lc][label] = select_neighbors(label, newNeighbors, M, lc);
            for(int neighbor : neighbors[lc][label]){
                neighbors[lc][neighbor].insert(label);
                if(neighbors[lc][neighbor].size() > M_max){
                    neighbors[lc][neighbor] = select_neighbors(neighbor, neighbors[lc][neighbor], M_max, lc);
                }
            }
            ep = nearest(label, newNeighbors);
        }

        // 4. 更新入口
        if(level > maxL){
            entry = label;
            maxL = level;
        }
    }

    /**
     * query:
     * query: the vector to be queried
     * k: the number of nearest neighbors to be returned
     * 
     * output:
     * a vector of labels of the k nearest neighbors
    */
    std::vector<int> HNSW::query(const int *query, int k) {
        
        //TODO 
        // 串行版本
        // 1. 初始化
        int label = -1;
        vector[label] = query;
        std::unordered_set<int> newNeighbors;
        int ep = entry;
        
        // 2. 从顶层搜索到第一层
        for(int lc = maxL; lc >= 1; lc--){
            std::unordered_set<int> w = search_layer(label, ep, 1, lc);
            newNeighbors.insert(w.begin(), w.end());
            ep = nearest(label,newNeighbors);
        }

        // 3. 对第零层操作
        newNeighbors = search_layer(label, ep, ef_construction, 0);
        if(ef_construction > k){
            newNeighbors = select_neighbors(label, newNeighbors, k, 0);
        }
        std::vector<int> res(newNeighbors.begin(), newNeighbors.end());
        return res;
    }
    
    /**
     * search_layer:
     * 将lc中的最近ef个节点加入到newNeighbors中
    */
    std::unordered_set<int> HNSW::search_layer(int label, int ep, int ef, int lc){
        std::unordered_set<int> visited; // 已访问节点
        visited.insert(ep);
        std::unordered_set<int> candidate; // 候选节点
        candidate.insert(ep);
        std::unordered_set<int> w; // 邻近节点集合
        w.insert(ep);
            while(candidate.size() > 0){
                int c = nearest(label, candidate);
                candidate.erase(c);
                int f = furthest(label, w);
                if(l2distance(vector[c], vector[label], dim) > l2distance(vector[f], vector[label], dim)){
                    break;
                }
                for(int e : neighbors[lc][c]){
                    if(visited.find(e) == visited.end()){
                        visited.insert(e);
                        f = furthest(label, w);
                        if(l2distance(vector[e], vector[label], dim) < l2distance(vector[f], vector[label], dim)
                            || w.size() < ef){
                                candidate.insert(e);
                                w.insert(e);
                                if(w.size() > ef){
                                    w.erase(furthest(label, w));
                                }
                            }
                    }
                }
            }
            return w;    
    }
    /**
     * nearest
     * 找到集合中离结点最近的结点
    */
    int HNSW::nearest(int label, std::unordered_set<int> newNeighbors){
        long min = LONG_MAX;
        long result;
        int nearest;
        for(int neighbor : newNeighbors){ // 遍历集合中的所有节点
            result = l2distance(vector[neighbor], vector[label], dim);
            if(result < min){
                nearest = neighbor;
                min = result;
            }
        }
        return nearest;
    }

    /**
     * furthest
     * 找到集合中离结点最远的结点
    */
    int HNSW::furthest(int label, std::unordered_set<int> newNeighbors){
        long max = 0;
        int furthest;
        long result;
        for(int neighbor : newNeighbors){
            result = l2distance(vector[neighbor], vector[label], dim);
            if(result > max){
                furthest = neighbor;
                max = result;
            }
        }
        return furthest;
    }

   /**
    * select_neighbors
    * 选择集合中最近的m个元素
   */

    std::unordered_set<int> HNSW::select_neighbors(int label, std::unordered_set<int> newNeighbors, int m, int lc){
        std::unordered_set<int> selected;
        for(int e : newNeighbors){
            if(selected.size() < m){ // 集合元素不够
                selected.insert(e);
                continue;
            }
            int f = furthest(label, selected);
            // 集合元素已满，并且是近邻的，需要插入
            if(l2distance(vector[e], vector[label], dim) < l2distance(vector[f], vector[label], dim)){ 
                selected.erase(f);
                selected.insert(e);
            }
        }
        return selected;
    }

    /**
     * concurrent_query:
     * 用于并行查询，此时label为thread_id，实现访问vectors的不同索引，不用加锁
    */
    std::vector<int> HNSW::concurrent_query(const int *query, int k, int thread_id)
    {
        int label = - thread_id;
        // 1. 初始化
        vector[label] = query;
        std::unordered_set<int> newNeighbors;
        int ep = entry;
        
        // 2. 从顶层搜索到第一层
        for(int lc = maxL; lc >= 1; lc--){
            std::unordered_set<int> w = search_layer(label, ep, 1, lc);
            newNeighbors.insert(w.begin(), w.end());
            ep = nearest(label,newNeighbors);
        }

        // 3. 对第零层操作
        newNeighbors = search_layer(label, ep, ef_construction, 0);
        if(ef_construction > k){
            newNeighbors = select_neighbors(label, newNeighbors, k, 0);
        }
        std::vector<int> res(newNeighbors.begin(), newNeighbors.end());
        return res;
    }
}