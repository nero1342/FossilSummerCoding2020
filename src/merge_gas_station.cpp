#include "merge_gas_station.hpp"
#include "dsu.hpp"

/*
Define A is the set(gas_station x) satisfy \forall x \exist y, dist(x, y) <= GAS_MAX
Return vector of A;
*/
vector<vector<int>> merge_set_gas_station(const vector<vector<int>> &map, int m, int n, int GAS_MAX) {
    //fprintf(stderr,"Merging....");
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
            //fprintf(stderr,"Dectected gas station %d at (%d, %d)\n", n_gas_station, i, j);
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
        ////fprintf(stderr,"At (%d, %d) with dist = %d:\n", u.x, u.y, dist[index_u]);
        for (int i = 0; i < 4; ++i) {
            Point v = u.add_dir(i);
            int index_v = Index::encode(v);
            if (index_v != -1 && map[v.x][v.y] != WALL) {
                if (dist[index_v] == -1) {
                    dist[index_v] = dist[index_u] + 1;
                    belong_to[index_v] = belong_to[index_u];
                    q.push(v);
                    ////fprintf(stderr,"--> New (%d, %d) with dist = %d\n", v.x, v.y, dist[index_v]);
                } else {
                    // dist(u) + 1 + dist(v) <= GAS_MAX
                    if (dist[index_u] + dist[index_v] < GAS_MAX) {
                        if (dsu->Union(belong_to[index_u], belong_to[index_v])) {
                            //fprintf(stderr,"--> Merge %d and %d at (%d, %d)...", belong_to[index_u], belong_to[index_v], v.x, v.y);
                            //fprintf(stderr,"Success\n");
                        }
                        // else {
                        //     //fprintf(stderr,"Fail, both are belong the same group\n");
                        // }
                    }
                }
            }
        }
    }
    return dsu -> convert2vector(gas_station);
}

pair<vector<int>, vector<int>> bfs_group_gas_station_to_star(int id, const vector<int> &set_gas, const vector<vector<int>>&map, int m, int n, int GAS_MAX, bool &reach_src, Point src) {
    fprintf(stderr,"Finding star can be reached by group gas station %d...\n", id);
    int total_star = 0;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) total_star += map[i][j] == STAR; // cout << map[i][j] << " \n"[j == n - 1];

    queue<Point> q;
    vector<int>dist(m * n, -1); // Dist of encode Point
    for (int index : set_gas) {
        dist[index] = 0;
        q.push(Index::decode(index));
    }
    vector<int> star, candidate_star;
    while (!q.empty()) {
        Point u = q.front(); q.pop();
        int index_u = Index::encode(u);
        if (map[u.x][u.y] == STAR) {
            if (dist[index_u] <= GAS_MAX) {
                if (dist[index_u] <= GAS_MAX / 2) {
                    //fprintf(stderr,"At (%d, %d) with dist = %d:\n", u.x, u.y, dist[index_u]);
                    star.push_back(index_u);
                    //fprintf(stderr,"-> Reach star %d...\n", (int)star.size());
                }
                else {
                    //fprintf(stderr,"At (%d, %d) with dist = %d:\n", u.x, u.y, dist[index_u]);
                    candidate_star.push_back(index_u);
                    //fprintf(stderr,"-> Reach candidate star %d...\n", (int)star.size());
                }
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
                ////fprintf(stderr,"--> New (%d, %d) with dist = %d\n", v.x, v.y, dist[index_v]);
            }
        }
    }
    if (star.size())
        fprintf(stderr,"Total stars can be reached: %d / %d\n", (int)star.size(), total_star);
    return make_pair(star, candidate_star);
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
        ////fprintf(stderr,"At (%d, %d) with dist = %d:\n", u.x, u.y, dist[index_u]);
        for (int i = 0; i < 4; ++i) {
            Point v = u.add_dir(i);
            int index_v = Index::encode(v);
            if (index_v != -1 && map[v.x][v.y] != WALL ) {
                //Point uu = Index::decode(u), vv = Index::decode(u);
                ////fprintf(stderr,"Edge connect (%d, %d) and (%d, %d)\n", u.x, u.y, v.x, v.y);
                if (dist[index_v] == -1) {
                    dist[index_v] = dist[index_u] + 1;
                    q.push(v);
                    belong_to[index_v] = belong_to[index_u];
                    edge[index_u].push_back(index_v);
                    edge[index_v].push_back(index_u);
                    ////fprintf(stderr,"--> New (%d, %d) with dist = %d\n", v.x, v.y, dist[index_v]);
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
