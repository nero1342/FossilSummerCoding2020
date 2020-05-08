#include "trace_path.hpp"

void dfs(int u, const vector<vector<int>> &edge, vector<int>&par, vector<int>&h, vector<int>&TimeIn) {
    static int cnt = 0;
    TimeIn[u] = ++cnt;
    for (int v : edge[u]) if (par[v] == -2) {
        par[v] = u;
        h[v] = h[u] + 1;
        dfs(v, edge, par, h, TimeIn);
    }
}

vector<int> Go(int src, int dest, const vector<int>& par, const vector<int>& h) {
    vector<int> path, path2;
    int x = src, y = dest;
    while (x != y) {
        if (h[x] <= h[y]) {
            path2.push_back(y);
            y = par[y];
        } else {
            path.push_back(x);
            x = par[x];
        }
    }
    path.push_back(x);
    while (path2.size()) {
        path.push_back(path2.back());
        path2.pop_back();
    }
    //Point uu = Index::decode(src), vv = Index::decode(dest);
    //fprintf(stderr,"Go from (%d, %d) to (%d, %d) with %d steps\n", uu.x, uu.y, vv.x, vv.y, (int)path.size());
    return path;
};
vector<int> Solve(int src, int dest, const vector<int> &belong_to,
                const vector<int>&par, const vector<int>&h,
                const vector<int>&par_gas, const vector<int>&h_gas) {
    Point uu = Index::decode(src), vv = Index::decode(dest);
    fprintf(stderr,"Solve from (%d, %d) to (%d, %d)...", uu.x, uu.y, vv.x, vv.y);
    int nearest_src = belong_to[src], nearest_dest = belong_to[dest];
    vector<int> path = Go(src, nearest_src, par, h); path.pop_back();
    vector<int> p_gas = Go(nearest_src, nearest_dest, par_gas, h_gas);
    for (int i = 0; i < (int)p_gas.size() - 1; ++i) {
        int u = p_gas[i], v = p_gas[i + 1];
        path += Go(u, v, par, h); path.pop_back();
    }
    path += Go(nearest_dest, dest, par, h);
    fprintf(stderr,"with %d steps\n", (int)path.size());
    return path;
}
