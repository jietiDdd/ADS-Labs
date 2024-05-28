#include <iostream>
#include <fstream>
#include "src/hnsw.hpp"
#include "util/vecs_io.hpp"
#include "util/ground_truth.hpp"
#include <thread>

using namespace std;
using namespace HNSWLab;

int main() {
    // 载入测试集
    std::printf("load ground truth\n");
    int gnd_n_vec = 100;
    int gnd_vec_dim = 10;
    char *path = "./data/siftsmall/gnd.ivecs";
    int *gnd = read_ivecs(gnd_n_vec, gnd_vec_dim, path);

    std::printf("load query\n");
    int query_n_vec = 100;
    int query_vec_dim = 128;
    path = "./data/siftsmall/query.bvecs";
    int *query = read_bvecs(query_n_vec, query_vec_dim, path);

    std::printf("load base\n");
    int base_n_vec = 10000;
    int base_vec_dim = 128;
    path = "./data/siftsmall/base.bvecs";
    int *base = read_bvecs(base_n_vec, base_vec_dim, path);

    // 生成hnsw并插入，进行插入的性能测试
    HNSW hnsw;

    printf("inserting\n");
    size_t report_every = 1000;
    double single_insert_time;
    TimeRecord insert_record;
    
    for(int i = 0; i < base_n_vec; i++){
        hnsw.insert(base + base_vec_dim * i, i);

        // 每插入1000个报告一次单次插入时延
        if(i % report_every == 0){
            single_insert_time = insert_record.get_elapsed_time_micro() / report_every * 1e-3;
            printf("from %d to %d: single insert time %.1f ms\n", i - 999, i, single_insert_time);
            insert_record.reset();
        }
    }

    // 串行的查询性能测试
    printf("serial querying\n");
    vector <vector<int>> test_gnd_l;
    double single_query_time;
    TimeRecord query_record;
    for (int i = 0; i < gnd_n_vec; i++) {
        vector<int> test_gnd = hnsw.query(query + i * query_vec_dim, gnd_vec_dim);
        test_gnd_l.push_back(test_gnd);
    }
    single_query_time = query_record.get_elapsed_time_micro() / query_n_vec * 1e-3;

    double recall = count_recall(gnd_n_vec, gnd_vec_dim, test_gnd_l, gnd);
    printf("average recall: %.3f, single query time %.1f ms\n", recall, single_query_time);

    // 并行的查询性能测试
    printf("current querying\n");
    vector<vector<int>> concurrent_test_gnd_l;
    double concurrent_single_query_time;
    TimeRecord concurrent_query_record;
    vector<std::thread> threads;
    for(int i = 0; i < gnd_n_vec; i++){
        threads.emplace_back([i, &hnsw, query, query_vec_dim, gnd_vec_dim, &concurrent_test_gnd_l](){
            vector<int> concurrent_test_gnd = hnsw.concurrent_query(query + i * query_vec_dim, gnd_vec_dim, i);
            concurrent_test_gnd_l[i] = concurrent_test_gnd;
        });
    }
    for(auto &thread: threads){
        thread.join();
    }
    concurrent_single_query_time = concurrent_query_record.get_elapsed_time_micro() / query_n_vec * 1e-3;

    double concurrent_recall = count_recall(gnd_n_vec, gnd_vec_dim, concurrent_test_gnd_l, gnd);
    printf("average recall: %.3f, single query time %.1f ms\n", concurrent_recall, concurrent_single_query_time);

    threads.clear();

    return 0;
}