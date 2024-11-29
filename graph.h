#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <vector>
#include <iostream>
#include "Spot.h"

class Graph
{
private:
    static const int vexNum_s = 6;  // 顶点数量
    static const int none_s = 9999; // 不连通的权重

    std::vector<Spot> spots;               // 景点数组
    std::vector<std::vector<double>> weight; // 邻接矩阵（动态二维vector）

    std::vector<std::vector<double>> distance; // Floyd 算法中用于存储距离
    std::vector<std::vector<int>> next;      // Floyd 算法中存储路径的中间点



public:
    std::vector<bool> visited;               // DFS 使用的访问标记
    std::vector<int> pathOfAll;             // 记录 DFS 路径

    // 参数化构造函数，从外部获取邻接矩阵
    Graph(const std::vector<Spot>& spots, const std::vector<std::vector<double>>& externalWeight);  // 参数化构造函数

    // DFS算法（用于推荐路径）
    void DFS(int start);
    void printDFSresult();

    // Floyd算法（用于查找两点间最短路径）
    void Floyd();
    void printFloydPath(int start, int end);

    // Prim算法（最小生成树，铺设道路）
    void PrimMST();

    // 打印图信息（调试用）
    void PrintGraph();
};

#endif // GRAPH_H
