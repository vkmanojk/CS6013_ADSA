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

struct DisjointSet {
    vector<int> parent;
    vector<int> rank;

    DisjointSet(int n) {
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
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, k;
    if (!(cin >> n >> m >> k))
        return 0;

    vector<Edge> edges(m + 1);
    for (int i = 1; i <= m; ++i) {
        cin >> edges[i].u >> edges[i].v >> edges[i].weight;
        edges[i].id = i;
        edges[i].is_mandatory = false;
    }

    for (int i = 0; i < k; ++i) {
        int road_id;
        cin >> road_id;
        edges[road_id].is_mandatory = true;
    }

    DisjointSet dsu(n);
    long long total_cost = 0;
    int edges_used = 0;

    for (int i = 1; i <= m; ++i) {
        if (edges[i].is_mandatory) {
            if (dsu.union_sets(edges[i].u, edges[i].v)) {
                total_cost += edges[i].weight;
                edges_used++;
            } else {
                cout << "-1\n";
                return 0;
            }
        }
    }

    vector<Edge> remaining_edges;
    for (int i = 1; i <= m; ++i)
        if (!edges[i].is_mandatory)
            remaining_edges.push_back(edges[i]);

    sort(remaining_edges.begin(), remaining_edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    for (const auto& edge : remaining_edges) {
        if (edges_used == n - 1)
            break;
        if (dsu.union_sets(edge.u, edge.v)) {
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
