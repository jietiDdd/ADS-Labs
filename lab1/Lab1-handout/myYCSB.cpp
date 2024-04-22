#include <iostream>
#include <set>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include "util.hpp"
#include "radix_tree.hpp"
#include "compressed_radix_tree.hpp"
#include "rb_tree_set.hpp"

void printLatency(std::vector<std::chrono::nanoseconds>& lat) {
    std::sort(lat.begin(), lat.end());
    std::cout << "Average latency: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::accumulate(lat.begin(), lat.end(), std::chrono::nanoseconds(0)) / lat.size()).count() << "ns" << std::endl;
    std::cout << "P50 latency: " << lat[static_cast<size_t>(lat.size() * 0.5)].count() << "ns" << std::endl;
    std::cout << "P90 latency: " << lat[static_cast<size_t>(lat.size() * 0.9)].count() << "ns" << std::endl;
    std::cout << "P99 latency: " << lat[static_cast<size_t>(lat.size() * 0.99)].count() << "ns" << std::endl;
}

void initial(Tree *tree) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dis(0, INT_MAX);
    for(int i = 0; i < 1000; i++) {
        tree->insert(dis(gen));
    }
}

void workload1(Tree *tree) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::chrono::nanoseconds> insert_lat;
    std::vector<std::chrono::nanoseconds> find_lat;

    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::seconds(30);
    std::uniform_int_distribution<int32_t> dis_zipf(0, INT_MAX);

    while(std::chrono::high_resolution_clock::now() < end) {
        auto op_start = std::chrono::high_resolution_clock::now();
        if(dis_zipf(gen) % 2 == 0) {
            tree->insert(zipf());
            insert_lat.push_back(std::chrono::high_resolution_clock::now() - op_start);
        } else {
            tree->find(zipf());
            find_lat.push_back(std::chrono::high_resolution_clock::now() - op_start);
        }
    }

    std::cout << "Insert: " <<std::endl;
    printLatency(insert_lat);
    std::cout << "Find: " <<std::endl;
    printLatency(find_lat);
}

void workload2(Tree *tree) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::chrono::nanoseconds> find_lat;

    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::seconds(30);

    while(std::chrono::high_resolution_clock::now() < end) {
        auto op_start = std::chrono::high_resolution_clock::now();
        tree->find(zipf());
        find_lat.push_back(std::chrono::high_resolution_clock::now() - op_start);
    }

    std::cout << "Find: " <<std::endl;
    printLatency(find_lat);
}

void workload3(Tree *tree) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::chrono::nanoseconds> insert_lat;
    std::vector<std::chrono::nanoseconds> find_lat;
    std::vector<std::chrono::nanoseconds> del_lat;

    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::seconds(30);
    std::uniform_int_distribution<int32_t> dis_zipf(0, INT_MAX);

    while(std::chrono::high_resolution_clock::now() < end) {
        auto op_start = std::chrono::high_resolution_clock::now();
        int32_t random = dis_zipf(gen);
        if(random % 4 == 0) {
            tree->insert(zipf());
            insert_lat.push_back(std::chrono::high_resolution_clock::now() - op_start);
        } else if(random % 4 == 1){
            tree->remove(zipf());
            del_lat.push_back(std::chrono::high_resolution_clock::now() - op_start);
        } else {
            tree->find(zipf());
            find_lat.push_back(std::chrono::high_resolution_clock::now() - op_start);
        }
    }

    std::cout << "Insert: " <<std::endl;
    printLatency(insert_lat);
    std::cout << "Remove: " <<std::endl;
    printLatency(del_lat);
    std::cout << "Find: " <<std::endl;
    printLatency(find_lat);
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    Tree *tree;

    // 基数树-workload1
    tree = new class RadixTree();
    std::cout << "Workload1: " << std::endl << "RadixTree: " << std::endl;
    initial(tree);
    workload1(tree);
    delete tree;

    // 压缩基数树-workload1
    tree = new class CompressedRadixTree();
    std::cout << "CompressedRadixTree: " << std::endl;
    initial(tree);
    workload1(tree);
    delete tree;

    // 红黑树-workload1
    tree = new class RedBlackTree();
    std::cout << "RedBlackTree: " << std::endl;
    initial(tree);
    workload1(tree);
    delete tree;

    // 基数树-workload2
    tree = new class RadixTree();
    std::cout << "Workload2: " << std::endl << "RadixTree: " << std::endl;
    initial(tree);
    workload2(tree);
    delete tree;

    // 压缩基数树-workload2
    tree = new class CompressedRadixTree();
    std::cout <<"CompressedRadixTree: " << std::endl;
    initial(tree);
    workload2(tree);
    delete tree;

    // 红黑树-workload2
    tree = new class RedBlackTree();
    std::cout <<"RedBlackTree: " << std::endl;
    initial(tree);
    workload2(tree);
    delete tree;

    // 基数树-workload3
    tree = new class RadixTree();
    std::cout << "Workload3: " << std::endl << "RadixTree: " << std::endl;
    initial(tree);
    workload3(tree);
    delete tree;

    // 压缩基数树-workload3
    tree = new class CompressedRadixTree();
    std::cout <<"CompressedRadixTree: " << std::endl;
    initial(tree);
    workload3(tree);
    delete tree;

    // 红黑树-workload3
    tree = new class RedBlackTree();
    std::cout <<"RedBlackTree: " << std::endl;
    initial(tree);
    workload3(tree);
    delete tree;
}
