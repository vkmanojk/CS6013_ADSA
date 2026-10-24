#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
    long long cost;
};

// Prim's algorithm with super-source node 0
long long prim_mst(int start_node, const vector<vector<Edge>>& adj, int n) {
    vector<long long> min_edge_cost(n + 1, INF);
    vector<bool> visited(n + 1, false);

    min_edge_cost[start_node] = 0;
    long long total_cost = 0;

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, start_node});

    while (!pq.empty()) {
        auto [weight, u] = pq.top();
        pq.pop();

        if (visited[u])
            continue;

        visited[u] = true;
        total_cost += weight;

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long edge_wt = edge.cost;
            if (!visited[v] && min_edge_cost[v] > edge_wt) {
                min_edge_cost[v] = edge_wt;
                pq.push({min_edge_cost[v], v});
            }
        }
    }

    return total_cost;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m))
        return 0;

    vector<vector<Edge>> adj(n + 1);

    for (int i = 1; i <= n; ++i) {
        long long station_cost;
        cin >> station_cost;
        adj[0].push_back({i, station_cost});
        adj[i].push_back({0, station_cost});
    }

    for (int i = 0; i < m; ++i) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        adj[u].push_back({v, c});
        adj[v].push_back({u, c});
    }

    long long min_total_cost = prim_mst(0, adj, n);
    cout << min_total_cost << "\n";

    return 0;
}
