#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = 1e18; 

struct Edge {
    int to;
    long long weight;
};

void dijkstra(int start, const vector<vector<Edge>>& adj, vector<long long>& d) {
    int n = adj.size();
    d.assign(n, INF);
    d[start] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> H;
    H.push({0, start});
    while (!H.empty()) {
        auto [dist_u, u] = H.top();
        H.pop(); 
        if (dist_u > d[u]) 
            continue;        
        for (const auto& edge : adj[u]) {
            int w = edge.to;
            long long weight = edge.weight;            
            if (d[u] + weight < d[w]) {
                d[w] = d[u] + weight;
                H.push({d[w], w}); 
            }
        }
    }
}

int main() {
    int N, M, S, T;
    if (!(cin >> N >> M >> S >> T)) 
        return 0;
    vector<vector<Edge>> adj(N + 1);
    int u_neg = -1, v_neg = -1;
    long long w_neg = 0;
    for (int i = 0; i < M; ++i) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        if (c < 0) {
            u_neg = u;
            v_neg = v;
            w_neg = c;
        } 
        else 
            adj[u].push_back({v, c});
    }
    vector<long long> d_S;
    dijkstra(S, adj, d_S);
    vector<long long> d_V;
    if (v_neg != -1) 
        dijkstra(v_neg, adj, d_V);    
    long long ans = d_S[T];
    if (u_neg != -1 && d_S[u_neg] != INF && d_V[T] != INF) {
        long long path_with_neg = d_S[u_neg] + w_neg + d_V[T];
        if (path_with_neg < ans) 
            ans = path_with_neg;
    }
    if (ans >= INF) 
        cout << "UNREACHABLE\n";
    else 
        cout << ans << "\n";
    return 0;
}