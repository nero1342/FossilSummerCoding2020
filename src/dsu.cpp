#include "dsu.hpp"

DisjointSetUnion::DisjointSetUnion(int n):f(n, -1){}

int DisjointSetUnion::root(int u) { return (f[u] < 0 ? u : f[u] = root(f[u])); }

bool DisjointSetUnion::Union(int u, int v) {
    u = root(u), v = root(v);
    if (u == v) return false;
    int t = f[u] + f[v];
    if (f[u] > f[v]) swap(u, v);
    f[u] = t, f[v] = u;
    return true;
}

vector<vector<int>> DisjointSetUnion::convert2vector(const vector<Point>& mapping) {
    int num = 0;
    vector<int> g(f.size());
    for (int i = 0; i < (int)f.size(); ++i) if (f[i] < 0) {
        g[i] = num++;
    }
    vector<vector<int>> result(num);
    for (int i = 0; i < (int)f.size(); ++i) {
        result[g[root(i)]].push_back(Index::encode(mapping[i]));
    }
    return result;
}