#include "PowerGrid.h"
#include <algorithm>
#include <cmath>
#include <iostream>

PowerGrid::PowerGrid(int n) : n(n), time(0) {
    graph.resize(n);
    visited.assign(n, false);
    disc.assign(n, -1);
    low.assign(n, -1);
    compId.assign(n, -1);
}

void PowerGrid::mapCity(const std::string& name, int index) {
    cityToIndex[name] = index;
}

int PowerGrid::getCityIndex(const std::string& name) const {
    return cityToIndex.at(name);
}

void PowerGrid::addLine(int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
    edges.emplace_back(u, v);
}

void PowerGrid::preprocess() {
    for (int i = 0; i < n; ++i) {
        if (!visited[i])
            tarjanDFS(i, -1);
    }

    buildBridgeTree();

    int treeSize = *max_element(compId.begin(), compId.end()) + 1;
    depth.resize(treeSize);
    up.assign(treeSize, std::vector<int>(20, -1));
    dfsBridgeTree(0, -1);
}

void PowerGrid::tarjanDFS(int u, int parent) {
    visited[u] = true;
    disc[u] = low[u] = time++;

    for (int v : graph[u]) {
        if (v == parent) continue;

        if (!visited[v]) {
            tarjanDFS(v, u);
            low[u] = std::min(low[u], low[v]);

            if (low[v] > disc[u]) {
                edges.push_back({u, v});
            }
        } else {
            low[u] = std::min(low[u], disc[v]);
        }
    }
}

void PowerGrid::buildBridgeTree() {
    visited.assign(n, false);
    int cid = 0;
    for (int i = 0; i < n; ++i) {
        if (!visited[i])
            dfsComponents(i, cid++);
    }

    bridgeTree.resize(cid);
    for (auto& e : edges) {
        int u = compId[e.first];
        int v = compId[e.second];
        if (u != v) {
            bridgeTree[u].push_back(v);
            bridgeTree[v].push_back(u);
        }
    }
}

void PowerGrid::dfsComponents(int u, int c) {
    visited[u] = true;
    compId[u] = c;

    for (int v : graph[u]) {
        if (visited[v]) continue;

        if (std::find(edges.begin(), edges.end(), std::make_pair(u, v)) == edges.end() &&
            std::find(edges.begin(), edges.end(), std::make_pair(v, u)) == edges.end()) {
            dfsComponents(v, c);
        }
    }
}

void PowerGrid::dfsBridgeTree(int u, int p) {
    up[u][0] = p;
    for (int i = 1; i < 20; ++i) {
        if (up[u][i - 1] != -1)
            up[u][i] = up[up[u][i - 1]][i - 1];
    }

    for (int v : bridgeTree[u]) {
        if (v != p) {
            depth[v] = depth[u] + 1;
            dfsBridgeTree(v, u);
        }
    }
}

int PowerGrid::lca(int u, int v) {
    if (depth[u] < depth[v])
        std::swap(u, v);

    for (int i = 19; i >= 0; --i) {
        if (up[u][i] != -1 && depth[up[u][i]] >= depth[v])
            u = up[u][i];
    }

    if (u == v)
        return u;

    for (int i = 19; i >= 0; --i) {
        if (up[u][i] != -1 && up[u][i] != up[v][i]) {
            u = up[u][i];
            v = up[v][i];
        }
    }

    return up[u][0];
}

int PowerGrid::query(const std::string& from, const std::string& to) {
    int u = compId[getCityIndex(from)];
    int v = compId[getCityIndex(to)];
    int ancestor = lca(u, v);
    return depth[u] + depth[v] - 2 * depth[ancestor];
}
