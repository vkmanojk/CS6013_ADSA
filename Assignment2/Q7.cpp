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

    int find_set(int v) {
        if (v == parent[v])
            return v;
        return parent[v] = find_set(parent[v]);
    }

    bool union_sets(int a, int b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            if (rank[a] < rank[b])
                swap(a, b);
            parent[b] = a;
            if (rank[a] == rank[b])
                rank[a]++;
            return true;
        }
        return false;
    }
};

int main() {
    int N, M, K;
    if (!(cin >> N >> M >> K)) 
        return 0;
        
    vector<Edge> edges(M + 1);
    for (int i = 1; i <= M; ++i) {
        cin >> edges[i].u >> edges[i].v >> edges[i].weight;
        edges[i].id = i;
        edges[i].is_mandatory = false;
    }
    
    vector<int> mandatory_ids(K);
    for (int i = 0; i < K; ++i) {
        cin >> mandatory_ids[i];
        edges[mandatory_ids[i]].is_mandatory = true;
    }
    
    DSU dsu(N);
    long long total_cost = 0;
    int edges_used = 0;
    
    for (int i = 1; i <= M; ++i) {
        if (edges[i].is_mandatory) {
            if (dsu.union_sets(edges[i].u, edges[i].v)) {
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
    for (int i = 1; i <= M; ++i) 
        if (!edges[i].is_mandatory) 
            remaining_edges.push_back(edges[i]);
        
    
    
    sort(remaining_edges.begin(), remaining_edges.end(), [](const Edge& a, const Edge& b) 
        {return a.weight < b.weight;});
    
    for (const auto& edge : remaining_edges) {
        if (edges_used == N - 1) 
            break;
        if (dsu.union_sets(edge.u, edge.v)) {
            total_cost += edge.weight;
            edges_used++;
        }
    }
    
    if (edges_used == N - 1)
        cout << total_cost << "\n";
    else
        cout << "-1\n";    
    return 0;
}