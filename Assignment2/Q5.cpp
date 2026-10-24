#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
    long long cost;
};

void dijkstra(int start, const vector<vector<Edge>>& adj, vector<long long>& dist) {
    int n = adj.size();
    dist.assign(n, INF);
    dist[start] = 0;

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, start});

    while(!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if(d > dist[u])
            continue;

        for(const auto& edge : adj[u]) {
            int v = edge.to;
            long long weight = edge.cost;
            if(dist[u]+weight < dist[v]) {
                dist[v] = dist[u]+weight;
                pq.push({dist[v], v});
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, s, t;
    if(!(cin>>n>>m>>s>>t))
        return 0;

    vector<vector<Edge>> adj(n+1);
    int u_neg = -1, v_neg = -1;
    long long w_neg = 0;

    for(int i = 0; i < m; ++i) {
        int u, v;
        long long c;
        cin>>u>>v>>c;
        if(c < 0) {
            u_neg = u;
            v_neg = v;
            w_neg = c;
        } else
            adj[u].push_back({v, c});
    }

    vector<long long> dist_from_s;
    dijkstra(s, adj, dist_from_s);

    vector<long long> dist_from_v;
    if(v_neg != -1)
        dijkstra(v_neg, adj, dist_from_v);

    long long min_cost = dist_from_s[t];

    if(u_neg != -1 && dist_from_s[u_neg] != INF && dist_from_v[t] != INF) {
        long long cost_via_shortcut = dist_from_s[u_neg]+w_neg+dist_from_v[t];
        if(cost_via_shortcut < min_cost)
            min_cost = cost_via_shortcut;
    }

    if(min_cost >= INF)
        cout<<"UNREACHABLE\n";
    else
        cout<<min_cost<<"\n";

    return 0;
}
