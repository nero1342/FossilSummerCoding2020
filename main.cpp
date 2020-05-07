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

    vector<vector<int>> convert2vector() {
        int num = 0;
        vector<int> g(f.size());
        for (int i = 0; i < (int)f.size(); ++i) if (f[i] < 0) {
            g[i] = num++;
        }
        vector<vector<int>> result(num);
        for (int i = 0; i < (int)f.size(); ++i) {
            result[g[root(i)]].push_back(i);
        }
        return result;
    }
};


/*
Define A is the set(gas_station x) satisfy \forall x \exist y, dist(x, y) <= GAS_MAX 
Return vector of A;
*/
vector<vector<int>> merge_set_gas_station(vector<vector<int>> map, int m, int n, int GAS_MAX) {
    printf("Merging....");
    // for (int i = 0; i < m; ++i) {
    //     for (int j = 0; j < n; ++j) cout << map[i][j] << " \n"[j == n - 1];
    // }
    queue<Point> q;
    vector<int>dist(m * n, -1); // Dist of encode Point
    vector<int>belong_to(m * n, -1); // Which union encode point belong to 
    int n_gas_station = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) if (map[i][j] == GAS_STATION) {
            printf("Dectected gas station %d at (%d, %d)\n", n_gas_station, i, j);
            q.push(Point(i, j));
            int index = Index::encode(Point(i, j));
            dist[index] = 0;
            belong_to[index] = n_gas_station;
            ++n_gas_station;
        }
    }
    DisjointSetUnion * dsu = new DisjointSetUnion(n_gas_station);
    while (!q.empty()) {
        Point u = q.front(); q.pop();
        int index_u = Index::encode(u);
        printf("At (%d, %d) with dist = %d:\n", u.x, u.y, dist[index_u]);
        for (int i = 0; i < 4; ++i) {
            Point v = u.add_dir(i);
            int index_v = Index::encode(v);
            if (index_v != -1) {
                if (dist[index_v] == -1) {
                    dist[index_v] = dist[index_u] + 1;
                    belong_to[index_v] = belong_to[index_u];
                    q.push(v);
                    printf("--> New (%d, %d) with dist = %d\n", v.x, v.y, dist[index_v]);
                } else {
                    // dist(u) + 1 + dist(v) <= GAS_MAX
                    if (dist[index_u] + dist[index_v] < GAS_MAX) {
                        printf("--> Merge %d and %d at (%d, %d)...", belong_to[index_u], belong_to[index_v], v.x, v.y);
                        (dsu->Union(belong_to[index_u], belong_to[index_v])) ? 
                            printf("Success\n") : printf("Fail, both are belong the same group\n");
                    } 
                }
            }
        }
    }
    return dsu -> convert2vector();
}

vector<int> bfs_group_gas_station_to_star(int id, vector<int> set_gas, vector<vector<int>>map, int m, int n, int GAS_MAX, bool &reach_src, Point src) {
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
        printf("At (%d, %d) with dist = %d:\n", u.x, u.y, dist[index_u]);
        if (map[u.x][u.y] == STAR) {
            star.push_back(index_u);
            printf("-> Reach star %d...\n", (int)star.size());
        }
        if (u == src) reach_src = true;
        if (dist[index_u] == GAS_MAX) continue;
        for (int i = 0; i < 4; ++i) {
            Point v = u.add_dir(i);
            int index_v = Index::encode(v);
            if (index_v != -1 && dist[index_v] == -1) {
                dist[index_v] = dist[index_u] + 1;
                q.push(v);
                printf("--> New (%d, %d) with dist = %d\n", v.x, v.y, dist[index_v]);
            }
        }
    }
    printf("Total stars can be reached: %d / %d\n", (int)star.size(), total_star);
    return star;
}

int main() {
    // Initialize 
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
        setStar[i].push_back(Index::encode(src));
        if (best_gas == -1 || (setStar[i].size() > setStar[best_gas].size())) best_gas = i;
    }
    if (best_gas == -1) {
        printf("There is no gas station, need another solution.\n");
        return 0;
    } 
    printf("*Choose gas station %d\n", best_gas);   
}