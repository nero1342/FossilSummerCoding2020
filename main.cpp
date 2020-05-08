#include <bits/stdc++.h>

using namespace std;

#define WALL 0
#define STREET 1
#define GAS_STATION 2
#define STAR 3
const int dir_row[4] = {1, 0, -1, 0};
const int dir_col[4] = {0, 1, 0, -1};
    
struct Point {
    int x, y;
    Point(int x = 0, int y = 0):x(x), y(y) {}
    void read() {
        cin >> x >> y;
        --x, --y;
    }
    Point add_dir(int i) {
        return Point(x + dir_row[i], y + dir_col[i]);
    }
    bool operator==(const Point &a) {
        return x == a.x && y == a.y;
    }
};
namespace Index{
    int m, n;
    void init(int _m, int _n) {
        m = _m, n = _n;
    }
    int encode(Point u) {
        int i = u.x, j = u.y;
        if (i < 0 || i >= m || j < 0 || j >= n) return -1;
        return i * n + j;
    }
    Point decode(int x) {
        return {x / n, x % n};
    }
};

struct DisjointSetUnion{
    vector<int> f, candidate;
    int val_candidate;
    DisjointSetUnion(int n):f(n, -1){}
    int root(int u) { return (f[u] < 0 ? u : f[u] = root(f[u])); }
    bool Union(int u, int v) {
        u = root(u), v = root(v);
        if (u == v) return false;
        int t = f[u] + f[v];
        if (f[u] > f[v]) swap(u, v);
        f[u] = t, f[v] = u;
        return true;
    }

    vector<vector<int>> convert2vector(const vector<Point>& mapping) {
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
};


/*
Define A is the set(gas_station x) satisfy \forall x \exist y, dist(x, y) <= GAS_MAX
Return vector of A;
*/
vector<vector<int>> merge_set_gas_station(const vector<vector<int>> &map, int m, int n, int GAS_MAX) {
    printf("Merging....");
    // for (int i = 0; i < m; ++i) {
    //     for (int j = 0; j < n; ++j) cout << map[i][j] << " \n"[j == n - 1];
    // }
    queue<Point> q;
    vector<int>dist(m * n, -1); // Dist of encode Point
    vector<int>belong_to(m * n, -1); // Which union encode point belong to
    int n_gas_station = 0;
    vector<Point> gas_station;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) if (map[i][j] == GAS_STATION) {
            printf("Dectected gas station %d at (%d, %d)\n", n_gas_station, i, j);
            q.push(Point(i, j));
            int index = Index::encode(Point(i, j));
            dist[index] = 0;
            belong_to[index] = n_gas_station;
            ++n_gas_station;
            gas_station.push_back(Point(i, j));
        }
    }
    DisjointSetUnion * dsu = new DisjointSetUnion(n_gas_station);
    while (!q.empty()) {
        Point u = q.front(); q.pop();
        int index_u = Index::encode(u);
        //printf("At (%d, %d) with dist = %d:\n", u.x, u.y, dist[index_u]);
        for (int i = 0; i < 4; ++i) {
            Point v = u.add_dir(i);
            int index_v = Index::encode(v);
            if (index_v != -1 && map[v.x][v.y] != WALL) {
                if (dist[index_v] == -1) {
                    dist[index_v] = dist[index_u] + 1;
                    belong_to[index_v] = belong_to[index_u];
                    q.push(v);
                    //printf("--> New (%d, %d) with dist = %d\n", v.x, v.y, dist[index_v]);
                } else {
                    // dist(u) + 1 + dist(v) <= GAS_MAX
                    if (dist[index_u] + dist[index_v] < GAS_MAX) {
                        if (dsu->Union(belong_to[index_u], belong_to[index_v])) {
                            printf("--> Merge %d and %d at (%d, %d)...", belong_to[index_u], belong_to[index_v], v.x, v.y);
                            printf("Success\n");
                        }
                        // else {
                        //     printf("Fail, both are belong the same group\n");
                        // }
                    }
                }
            }
        }
    }
    return dsu -> convert2vector(gas_station);
}

vector<int> bfs_group_gas_station_to_star(int id, const vector<int> &set_gas, const vector<vector<int>>&map, int m, int n, int GAS_MAX, bool &reach_src, Point src) {
    printf("Finding star can be reached by group gas station %d...\n", id);
    int total_star = 0;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) total_star += map[i][j] == STAR; // cout << map[i][j] << " \n"[j == n - 1];

    queue<Point> q;
    vector<int>dist(m * n, -1); // Dist of encode Point
    for (int index : set_gas) {
        dist[index] = 0;
        q.push(Index::decode(index));
    }
    vector<int> star;
    while (!q.empty()) {
        Point u = q.front(); q.pop();
        int index_u = Index::encode(u);
        if (map[u.x][u.y] == STAR) {
            if (dist[index_u] <= GAS_MAX / 2) {
                printf("At (%d, %d) with dist = %d:\n", u.x, u.y, dist[index_u]);
                star.push_back(index_u);
                printf("-> Reach star %d...\n", (int)star.size());
            }
        }
        if (u == src) reach_src = true;
        if (dist[index_u] == GAS_MAX) continue;
        for (int i = 0; i < 4; ++i) {
            Point v = u.add_dir(i);
            int index_v = Index::encode(v);
            if (index_v != -1 && map[v.x][v.y] != WALL && dist[index_v] == -1) {
                dist[index_v] = dist[index_u] + 1;
                q.push(v);
                //printf("--> New (%d, %d) with dist = %d\n", v.x, v.y, dist[index_v]);
            }
        }
    }
    printf("Total stars can be reached: %d / %d\n", (int)star.size(), total_star);
    return star;
}

vector<vector<int>> build_graph_gas_and_star(const vector<int>& setGasStation, const vector<vector<int>>&map, int m, int n, int GAS_MAX, vector<int> &belong_to, vector<vector<int>>& graph_gas) {
    graph_gas.resize(m * n);
    belong_to.resize(m * n, -1);
    vector<vector<int>> edge(m * n);
    vector<int> dist(m * n, -1);
    queue<Point> q;
    for (int i = 0; i < (int)setGasStation.size(); ++i) {
        auto x = setGasStation[i];
        q.push(Index::decode(x));
        dist[x] = 0;
        belong_to[x] = x;
    }
    DisjointSetUnion * dsu = new DisjointSetUnion(m * n);
    while (!q.empty()) {
        Point u = q.front(); q.pop();
        int index_u = Index::encode(u);
        //printf("At (%d, %d) with dist = %d:\n", u.x, u.y, dist[index_u]);
        for (int i = 0; i < 4; ++i) {
            Point v = u.add_dir(i);
            int index_v = Index::encode(v);
            if (index_v != -1 && map[v.x][v.y] != WALL ) {
                //Point uu = Index::decode(u), vv = Index::decode(u);
                //printf("Edge connect (%d, %d) and (%d, %d)\n", u.x, u.y, v.x, v.y);
                if (dist[index_v] == -1) {
                    dist[index_v] = dist[index_u] + 1;
                    q.push(v);
                    belong_to[index_v] = belong_to[index_u];
                    edge[index_u].push_back(index_v);
                    edge[index_v].push_back(index_u);
                    //printf("--> New (%d, %d) with dist = %d\n", v.x, v.y, dist[index_v]);
                } else {
                    if (dist[index_v] + dist[index_u] < GAS_MAX) {
                        if (dsu -> Union(belong_to[index_u], belong_to[index_v])) {
                            edge[index_u].push_back(index_v);
                            edge[index_v].push_back(index_u);
                            graph_gas[belong_to[index_u]].push_back(belong_to[index_v]);
                            graph_gas[belong_to[index_v]].push_back(belong_to[index_u]);
                        }
                    }
                }
            }
        }
    }
    return edge;
}

void dfs(int u, const vector<vector<int>> &edge, vector<int>&par, vector<int>&h, vector<int>&TimeIn) {
    static int cnt = 0;
    TimeIn[u] = ++cnt;
    for (int v : edge[u]) if (par[v] == -2) {
        par[v] = u;
        h[v] = h[u] + 1;
        dfs(v, edge, par, h, TimeIn);
    }
}

vector<int> operator+(const vector<int>&a, const vector<int> &b) {
    vector<int> c;
    for (int x:a) c.push_back(x);
    for (int x:b) c.push_back(x);
    return c;
}
void operator+=(vector<int>&a, const vector<int> &b) {
    for (int x:b) a.push_back(x);
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
    //printf("Go from (%d, %d) to (%d, %d) with %d steps\n", uu.x, uu.y, vv.x, vv.y, (int)path.size());
    return path;
};
vector<int> Solve(int src, int dest, const vector<int> &belong_to,
                const vector<int>&par, const vector<int>&h,
                const vector<int>&par_gas, const vector<int>&h_gas) {
    printf("Solve from %d to %d...", src, dest);
    int nearest_src = belong_to[src], nearest_dest = belong_to[dest];
    vector<int> path = Go(src, nearest_src, par, h); path.pop_back();
    vector<int> p_gas = Go(nearest_src, nearest_dest, par_gas, h_gas);
    for (int i = 0; i < (int)p_gas.size() - 1; ++i) {
        int u = p_gas[i], v = p_gas[i + 1];
        path += Go(u, v, par, h); path.pop_back();
    }
    path += Go(nearest_dest, dest, par, h);
    printf("with %d steps\n", (int)path.size());
    return path;
}

int main() {
    // Initialize
    freopen("map_demo.txt","r",stdin);
    Point src;
    src.read();
    int GAS_MAX, m, n;
    cin >> GAS_MAX >> m >> n;
    vector<vector<int>> map(m, vector<int>(n));
    Index::init(m, n);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) cin >> map[i][j];
    }

    // Merge gas station
    vector<vector<int>> setGasStation = merge_set_gas_station(map, m, n, GAS_MAX);
    printf("There are %d set of groups of gas stations.\n", (int)setGasStation.size());
    for (int i = 0; i < (int) setGasStation.size(); ++i) {
        printf("Set %d has %d gas stations:\n", i, (int)setGasStation[i].size());
        for (int index: setGasStation[i]) {
            Point u = Index::decode(index);
            printf("(%d, %d), ", u.x, u.y);
        }
        putchar('\n');
    }

    // With each set of gas station, find set of star, dist(station, star) <= k
    vector<vector<int>> setStar(setGasStation.size());
    int best_gas = -1;
    for (int i = 0; i < (int) setGasStation.size(); ++i) {
        bool reach_src = false;
        // Can optimize by bfs from src first, and just focus on group gas stations that can be reached.
        setStar[i] = bfs_group_gas_station_to_star(i, setGasStation[i], map, m, n, GAS_MAX, reach_src, src);
        if (!reach_src) continue;
        if (best_gas == -1 || (setStar[i].size() > setStar[best_gas].size())) best_gas = i;
    }
    if (best_gas == -1) {
        printf("There is no gas station, need another solution.\n");
        return 0;
    }
    printf("*Choose gas station %d\n", best_gas);
    vector<int> belong_to;
    vector<vector<int>> graph_gas;// Spanning tree of best gas
    vector<vector<int>> edge = build_graph_gas_and_star(setGasStation[best_gas], map, m, n, GAS_MAX, belong_to, graph_gas);
    vector<int> par(m * n, -2), h(m * n, 0), par_gas(m * n, -2), h_gas(m * n, 0);
    par[belong_to[Index::encode(src)]] = par_gas[Index::encode(src)] = -1;
    vector<int> TimeIn(m * n);
    dfs(belong_to[Index::encode(src)], edge, par, h, TimeIn);
    dfs(belong_to[Index::encode(src)], graph_gas, par_gas, h_gas, TimeIn);
    sort(setStar[best_gas].begin(), setStar[best_gas].end(), [&](int a, int b) {
        return TimeIn[belong_to[a]] > TimeIn[belong_to[b]];
    });
    // exit(0);
    ofstream out("path.txt");
    int index_src = Index::encode(src);
    while (setStar[best_gas].size()) {
        int index_star = setStar[best_gas].back();
        setStar[best_gas].pop_back();
        vector<int> path = Solve(index_src, index_star, belong_to, par, h, par_gas, h_gas);
        for (int i = 0; i < (int)path.size(); ++i) {
            int id = path[i];
            Point pt = Index::decode(id);
            if (i == (int)path.size() - 1) {
                if (setStar[best_gas].empty())
                    out << pt.x + 1 << ' ' << pt.y + 1 << '\n';
            } else {
                out << pt.x + 1 << ' ' << pt.y + 1 << '\n';
            }
        }
        index_src = index_star;
    }
    out.close();
}
