#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct TwoSatSolver {
    int n;
    vector<vector<int>> adj, adj_rev;
    vector<int> order, comp_id, assignment;
    vector<bool> visited;
    int comp_count;

    TwoSatSolver(int num_vars) : n(num_vars) {
        adj.resize(2*n);
        adj_rev.resize(2*n);
        comp_id.resize(2*n);
        assignment.resize(n);
    }

    void add_implication(int u, int v) {
        adj[u].push_back(v);
        adj_rev[v].push_back(u);
    }

    void add_clause(int u, bool u_pos, int v, bool v_pos) {
        int u_lit = u*2+(u_pos ? 0 : 1);
        int v_lit = v*2+(v_pos ? 0 : 1);
        int not_u = u_lit^1;
        int not_v = v_lit^1;
        add_implication(not_u, v_lit);
        add_implication(not_v, u_lit);
    }

    // Iterative post-order DFS to prevent stack overflow on deep graphs
    void dfs1(int start_node) {
        vector<pair<int, int>> st;
        st.push_back({start_node, 0});
        visited[start_node] = true;
        while(!st.empty()) {
            int u = st.back().first;
            int &edge_idx = st.back().second;
            if(edge_idx < (int)adj[u].size()) {
                int v = adj[u][edge_idx++];
                if(!visited[v]) {
                    visited[v] = true;
                    st.push_back({v, 0});
                }
            } else {
                order.push_back(u);
                st.pop_back();
            }
        }
    }

    // Iterative reachability on reversed graph
    void dfs2(int start_node, int c) {
        vector<int> st = {start_node};
        comp_id[start_node] = c;
        while(!st.empty()) {
            int u = st.back();
            st.pop_back();
            for(int v : adj_rev[u])
                if(comp_id[v] == -1) {
                    comp_id[v] = c;
                    st.push_back(v);
                }
        }
    }

    bool solve() {
        visited.assign(2*n, false);
        for(int i = 0; i < 2*n; ++i)
            if(!visited[i])
                dfs1(i);

        comp_id.assign(2*n, -1);
        comp_count = 0;
        for(int i = 2*n-1; i >= 0; --i) {
            int u = order[i];
            if(comp_id[u] == -1)
                dfs2(u, comp_count++);
        }

        for(int i = 0; i < n; ++i) {
            if(comp_id[2*i] == comp_id[2*i+1])
                return false;
            assignment[i] = comp_id[2*i] > comp_id[2*i+1];
        }
        return true;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    if(cin>>t) {
        while(t--) {
            int n, m;
            cin>>n>>m;
            TwoSatSolver solver(n);

            for(int i = 0; i < m; ++i) {
                int u, v;
                cin>>u>>v;
                solver.add_clause(abs(u)-1, (u > 0), abs(v)-1, (v > 0));
            }

            if(solver.solve()) {
                for(int i = 0; i < n; ++i)
                    cout<<solver.assignment[i]<<(i == n-1 ? "" : " ");
                cout<<"\n";
            } else
                cout<<"Unsatisfiable\n";
        }
    }
    return 0;
}
