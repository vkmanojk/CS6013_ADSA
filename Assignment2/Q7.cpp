#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

struct Edge {
    int u, v;
    long long weight;
    int id;
    bool is_mandatory;
};

struct DSU {
    vector<int> parent;
    vector<int> rank;

    DSU(int n) {
        parent.resize(n + 1);
        rank.assign(n + 1, 0);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int i) {
        if (i == parent[i]) 
            return i;
        return parent[i] = find(parent[i]);
    }

    bool union_set(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        
        if (root_i != root_j) {
            if (rank[root_i] < rank[root_j]) 
                swap(root_i, root_j);
            parent[root_j] = root_i;
            if (rank[root_i] == rank[root_j]) 
                rank[root_i]++;
            return true;
        }
        return false;
    }
};

int main() {
    int n, m, k;
    if (!(cin >> n >> m >> k)) 
        return 0;
        
    vector<Edge> edges(m + 1);
    for (int i = 1; i <= m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].weight;
        edges[i].id = i;
        edges[i].is_mandatory = false;
    }
    
    for (int i = 0; i < k; i++) {
        int req_id;
        cin >> req_id;
        edges[req_id].is_mandatory = true;
    }
    
    DSU dsu(n);
    long long total_cost = 0;
    int edges_used = 0;
    
    for (int i = 1; i <= m; i++) {
        if (edges[i].is_mandatory) {
            if (dsu.union_set(edges[i].u, edges[i].v)) {
                total_cost += edges[i].weight;
                edges_used++;
            } 
            else {
                cout << "-1\n";
                return 0;
            }
        }
    }
    
    vector<Edge> remaining_edges;
    for (int i = 1; i <= m; i++) 
        if (!edges[i].is_mandatory) 
            remaining_edges.push_back(edges[i]);
    
    sort(remaining_edges.begin(), remaining_edges.end(), [](const Edge& a, const Edge& b) 
        {return a.weight < b.weight;});
    
    for (const auto& edge : remaining_edges) {
        if (edges_used == n - 1) 
            break;
        if (dsu.union_set(edge.u, edge.v)) {
            total_cost += edge.weight;
            edges_used++;
        }
    }
    
    if (edges_used == n - 1)
        cout << total_cost << "\n";
    else
        cout << "-1\n";
    
    return 0;
}