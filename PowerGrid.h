#ifndef POWERGRID_H
#define POWERGRID_H

#include <vector>
#include <string>
#include <unordered_map>

class PowerGrid {
public:
    PowerGrid(int n);
    void addLine(int u, int v);
    void preprocess();
    int query(const std::string& from, const std::string& to);
    void mapCity(const std::string& name, int index);
    int getCityIndex(const std::string& name) const;

private:
    int n, time;
    std::vector<std::vector<int>> graph;
    std::vector<std::pair<int, int>> edges;
    std::vector<bool> visited;
    std::vector<int> disc, low;
    std::vector<std::vector<int>> bridgeTree;
    std::vector<int> compId;
    std::vector<std::vector<int>> up;
    std::vector<int> depth;

    std::unordered_map<std::string, int> cityToIndex;

    void tarjanDFS(int u, int parent);
    void buildBridgeTree();
    void dfsBridgeTree(int u, int p);
    int lca(int u, int v);
    void dfsComponents(int u, int c);
};

#endif
