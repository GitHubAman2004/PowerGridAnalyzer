#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <algorithm>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

map<string, int> cityToId;
vector<string> idToCity;
vector<vector<int>> adj;
set<pair<int, int>> bridges;
vector<bool> visited;
vector<int> tin, low;
int timer;

void dfs(int v, int p = -1) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    for (int to : adj[v]) {
        if (to == p) continue;
        if (visited[to]) {
            low[v] = min(low[v], tin[to]);
        } else {
            dfs(to, v);
            low[v] = min(low[v], low[to]);
            if (low[to] > tin[v]) {
                bridges.insert({min(v, to), max(v, to)});
            }
        }
    }
}

int countBridgesOnPath(int u, int v) {
    stack<int> st;
    set<int> visitedNodes;
    map<int, int> parent;
    st.push(u);
    visitedNodes.insert(u);

    while (!st.empty()) {
        int curr = st.top(); st.pop();
        if (curr == v) break;
        for (int nei : adj[curr]) {
            if (!visitedNodes.count(nei)) {
                visitedNodes.insert(nei);
                parent[nei] = curr;
                st.push(nei);
            }
        }
    }

    int count = 0;
    while (v != u) {
        int p = parent[v];
        if (bridges.count({min(v, p), max(v, p)})) count++;
        v = p;
    }
    return count;
}

int main() {
    json input;
    std::ifstream("input.json") >> input;
    auto cities = input["cities"];
    auto edges = input["edges"];
    auto queries = input["queries"];

    int n = cities.size();
    cityToId.clear();
    idToCity.clear();
    adj.assign(n, {});
    bridges.clear();

    for (int i = 0; i < n; ++i) {
        cityToId[cities[i]] = i;
        idToCity.push_back(cities[i]);
    }

    for (auto& e : edges) {
        int u = cityToId[e[0]];
        int v = cityToId[e[1]];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    visited.assign(n, false);
    tin.assign(n, -1);
    low.assign(n, -1);
    timer = 0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) dfs(i);
    }

    json output;
    output["results"] = json::array();
    for (auto& q : queries) {
        int u = cityToId[q[0]];
        int v = cityToId[q[1]];
        output["results"].push_back(countBridgesOnPath(u, v));
    }

    // Include bridges in output
    json bridgesArr = json::array();
    for (auto& b : bridges) {
        bridgesArr.push_back({idToCity[b.first], idToCity[b.second]});
    }
    output["bridges"] = bridgesArr;

    // Write to output.json
    ofstream("output.json") << output.dump(2);
    return 0;
}
