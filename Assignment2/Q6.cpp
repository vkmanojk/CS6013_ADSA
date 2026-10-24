#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = 1e18; 

struct Edge {
    int to;
    long long weight;
};

// Prim's algorithm 
long long prim(int start, const vector<vector<Edge>>& adj, int n) {
    vector<long long> label(n + 1, INF);
    vector<int> S(n + 1, 0); 
    
    label[start] = 0;
    long long total_cost = 0;
    
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> H;
    H.push({0, start});
    
    while(!H.empty()) {
        auto [wt_v, v] = H.top();
        H.pop();
        
        if (S[v] == 1) 
            continue;        
        
        S[v] = 1;
        total_cost += wt_v;
        
        for (const auto& edge : adj[v]) {
            int w = edge.to;
            long long weight = edge.weight;                        
            if (S[w] == 0 && label[w] > weight) {
                label[w] = weight;
                H.push({label[w], w}); 
            }
        }
    }
    
    return total_cost;
}

int main() {
    int N, M;
    if (!(cin >> N >> M)) 
        return 0;

    vector<vector<Edge>> adj(N + 1);
    
    for (int i = 1; i <= N; ++i) {
        long long P_i;
        cin >> P_i;
        adj[0].push_back({i, P_i});
        adj[i].push_back({0, P_i});
    }
    
    for (int i = 0; i < M; ++i) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        adj[u].push_back({v, c});
        adj[v].push_back({u, c});
    }
    
    long long ans = prim(0, adj, N);
    cout << ans << "\n";
    return 0;
}