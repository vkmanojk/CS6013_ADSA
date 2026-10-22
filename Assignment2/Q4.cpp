#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct TwoSAT {
    int n;
    vector<vector<int>> adj, adj_rev;
    vector<int> order, scc, assignment;
    vector<bool> visited;
    int scc_cnt;

    TwoSAT(int n) : n(n) {
        adj.resize(2 * n);
        adj_rev.resize(2 * n);
        scc.resize(2 * n);
        assignment.resize(n);
    }

    void add_implication(int u, int v) {
        adj[u].push_back(v);
        adj_rev[v].push_back(u);
    }

    void add_clause(int u, bool u_is_pos, int v, bool v_is_pos) {
        int u_idx = u * 2 + (u_is_pos ? 0 : 1);
        int v_idx = v * 2 + (v_is_pos ? 0 : 1);
        int not_u = u_idx ^ 1;
        int not_v = v_idx ^ 1;        
        add_implication(not_u, v_idx);
        add_implication(not_v, u_idx);
    }

    void dfs1(int u) {
        visited[u] = true;
        for (int v : adj[u]) 
            if (!visited[v]) 
                dfs1(v);
        order.push_back(u);
    }

    void dfs2(int u, int c) {
        scc[u] = c;
        for (int v : adj_rev[u]) 
            if (scc[v] == -1) dfs2(v, c);
    }

    bool solve() {
        visited.assign(2 * n, false);
        for (int i = 0; i < 2 * n; ++i) 
            if (!visited[i]) 
                dfs1(i);
        scc.assign(2 * n, -1);
        scc_cnt = 0;        
        for (int i = 2 * n - 1; i >= 0; --i) {
            int u = order[i];
            if (scc[u] == -1) 
                dfs2(u, scc_cnt++);
        }
        for (int i = 0; i < n; ++i) {
            if (scc[2 * i] == scc[2 * i + 1])
                return false; 
            assignment[i] = scc[2 * i] > scc[2 * i + 1];
        }
        return true;
    }
};

int main() {
    int t, n, m;
    if (cin >> t) {
        while (t--) {
            cin >> n >> m;
            TwoSAT ts(n);
            for (int i = 0; i < m; ++i) {
                int u, v;
                cin >> u >> v;                
                bool u_pos = (u > 0);
                bool v_pos = (v > 0);
                ts.add_clause(abs(u) - 1, (u > 0), abs(v) - 1, (v > 0));
            }
            if (ts.solve()) {
                for (int i = 0; i < n; ++i) {
                    cout << ts.assignment[i] << (i == n - 1 ? "" : " ");
                }
                cout << "\n";
            } else 
                cout << "Unsatisfiable\n";
        }
    }
    return 0;
}