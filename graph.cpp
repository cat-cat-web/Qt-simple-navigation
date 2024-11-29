#include"graph.h"
#include<QMessageBox>


// 参数化构造函数
Graph::Graph(const std::vector<Spot>& spots, const std::vector<std::vector<double>>& externalWeight) {
    this->spots = spots;
    weight = externalWeight;

}

// DFS算法 (用于推荐游览路线）
void Graph::DFS(int start)
{

    if (visited[start]) return;

    // 当前节点已访问
    visited[start] = true;
    pathOfAll.push_back(start);

    for (size_t i = 0; i < spots.size(); ++i) {
        // 有路径且未访问
        if (weight[start][i] != none_s && !visited[i]) {
            DFS(i);
        }
    }
}

// 打印 DFS 路径结果
void Graph::printDFSresult()
{

    QString pathStr = "推荐游览路线: ";
    for (size_t i = 0; i < pathOfAll.size(); ++i) {
        // 使用 pathOfAll 中的索引来反向查找 Spot 名字
        int index = pathOfAll[i];
        if (index >= 0 && index < spots.size()) {
            pathStr += spots[index].getName();
        } else {
            pathStr += "Invalid Spot";
        }

        if (i < pathOfAll.size() - 1) {
            pathStr += " -> ";
        }
    }


    QMessageBox::information(nullptr, "推荐游览路线", pathStr);
}



// Floyd算法 （用于查找两点之间最短路径）
void Graph::Floyd() {

    distance.resize(vexNum_s, std::vector<double>(vexNum_s, none_s));
    next.resize(vexNum_s, std::vector<int>(vexNum_s, -1));

    // 初始化
    for (size_t i = 0; i < vexNum_s; ++i) {
        for (size_t j = 0; j < vexNum_s; ++j) {
            if (i == j) {
                distance[i][j] = 0; // 自己到自己为 0
                next[i][j] = -1; // 自己到自己没有中介点
            } else if (weight[i][j] != none_s) {
                distance[i][j] = weight[i][j]; // 直接路径的距离
                next[i][j] = j; // 下一跳是目标点
            }
        }
    }

    // Floyd 算法
    for (size_t k = 0; k < vexNum_s; ++k) {
        for (size_t i = 0; i < vexNum_s; ++i) {
            for (size_t j = 0; j < vexNum_s; ++j) {
                // 更新最短路径；有新路径且长度小于当前
                if (distance[i][k] != none_s && distance[k][j] != none_s &&
                    distance[i][k] + distance[k][j] < distance[i][j])
                {
                    distance[i][j] = distance[i][k] + distance[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}


// 打印 Floyd 路径结果
void Graph::printFloydPath(int start, int end) {
    if (next[start][end] == -1) {
        QMessageBox::information(nullptr, "无道路", "两个景点不通");
        return;
    }

    QString pathStr = "从 " + spots[start].getName() + " 到 " + spots[end].getName() + ": ";
    int current = start;
    while (current != end) {
        pathStr += spots[current].getName() + " -> ";

        current = next[current][end];
    }
    pathStr += spots[end].getName() + " (总距离: " + QString::number(distance[start][end]) + ")";


    QMessageBox::information(nullptr, "两点之间最短路径", pathStr);
}

// Prim 最小生成树算法
void Graph::PrimMST()
{
    size_t size = spots.size();
    std::vector<double> minCost(size, none_s);
    std::vector<bool> inMST(size, false);
    std::vector<int> parent(size, -1); //记录每个节点的父节点以重建
    double totalWeight = 0;

    minCost[0] = 0; //从0点开始

    for (size_t count = 0; count < size; ++count) {
        int u = -1;
        double min = none_s;
        for (size_t i = 0; i < size; ++i) {
            if (!inMST[i] && minCost[i] < min) {
                min = minCost[i];
                u = i;
            }
        }

        if (u == -1) {
            std::cout << "Graph is not connected!" << std::endl;
            return;
        }

        inMST[u] = true;
        totalWeight += minCost[u];

        //找与之相连的、未被访问过的、边权最小的点
        for (size_t v = 0; v < size; ++v) {
            if (!inMST[v] && weight[u][v] != none_s && weight[u][v] < minCost[v]) {
                minCost[v] = weight[u][v];
                parent[v] = u;
            }
        }
    }

    std::cout << "Minimum Spanning Tree:" << std::endl;
    for (size_t i = 1; i < size; ++i) {
        std::cout << "Edge: " << parent[i] << " - " << i << ", Weight: " << weight[parent[i]][i] << std::endl;
    }
    std::cout << "Total Weight: " << totalWeight << std::endl;
}


//打印图，调试用
void Graph::PrintGraph() {
    // 打印 Spots
    std::cout << "Spots in the graph:" << std::endl;
    for (size_t i = 0; i < spots.size(); ++i) {
        std::cout << "Spot " << i << ": " << spots[i].getName().toStdString() << std::endl;
    }

    // 打印 Weight 矩阵
    std::cout << "\nWeight matrix:" << std::endl;
    for (size_t i = 0; i < spots.size(); ++i) {
        for (size_t j = 0; j < spots.size(); ++j) {

                std::cout << weight[i][j] << " ";
            }

        std::cout << std::endl;
    }
}
