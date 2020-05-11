#include "define.hpp"
#include "merge_gas_station.hpp"
#include "trace_path.hpp"
#include <ctime>

int Index::m;
int Index::n;

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
    auto t1 = std::chrono::steady_clock::now();
    // Merge gas station
    vector<vector<int>> setGasStation = merge_set_gas_station(map, m, n, GAS_MAX);
    //fprintf(stderr,"There are %d set of groups of gas stations.\n", (int)setGasStation.size());
    for (int i = 0; i < (int) setGasStation.size(); ++i) {
        //fprintf(stderr,"Set %d has %d gas stations:\n", i, (int)setGasStation[i].size());
        for (int index: setGasStation[i]) {
            Point u = Index::decode(index);
            //fprintf(stderr,"(%d, %d), ", u.x, u.y);
        }
        //fprintf(stderr,"\n");
    }

    // With each set of gas station, find set of star, dist(station, star) <= k
    vector<vector<int>> setStar(setGasStation.size()), setCandidateStar(setGasStation.size());
    int best_gas = -1;
    for (int i = 0; i < (int) setGasStation.size(); ++i) {
        bool reach_src = false;
        // Can optimize by bfs from src first, and just focus on group gas stations that can be reached.
        auto [star, candidate_star] = bfs_group_gas_station_to_star(i, setGasStation[i], map, m, n, GAS_MAX, reach_src, src);
        setStar[i] = star, setCandidateStar[i] = candidate_star;
        if (!reach_src) continue;
        if (best_gas == -1 || (setStar[i].size() > setStar[best_gas].size())) best_gas = i;
    }


    //
    if (best_gas == -1) {
        //fprintf(stderr,"There is no gas station, need another solution.\n");
        return 0;
    }
    fprintf(stderr,"*Choose gas station %d\n", best_gas);
    bool reach_src;
    auto [star, candidate_star] = bfs_group_gas_station_to_star(best_gas, setGasStation[best_gas], map, m, n, GAS_MAX, reach_src, src);
    // Initialize graph to trace path + heuristic to reduce path length.
    vector<int> belong_to;
    vector<vector<int>> graph_gas;// Spanning tree of best gas
    vector<vector<int>> edge = build_graph_gas_and_star(setGasStation[best_gas], map, m, n, GAS_MAX, belong_to, graph_gas);
    vector<int> par(m * n, -2), h(m * n, 0), par_gas(m * n, -2), h_gas(m * n, 0);
    vector<int> TimeIn(m * n);
    par[belong_to[Index::encode(src)]] = par_gas[Index::encode(src)] = -1;
    dfs(belong_to[Index::encode(src)], edge, par, h, TimeIn);
    dfs(belong_to[Index::encode(src)], graph_gas, par_gas, h_gas, TimeIn);
    sort(setStar[best_gas].begin(), setStar[best_gas].end(), [&](int a, int b) {
        return TimeIn[belong_to[a]] > TimeIn[belong_to[b]];
    });
    sort(setCandidateStar[best_gas].begin(), setCandidateStar[best_gas].end(), [&](int a, int b) {
        return TimeIn[belong_to[a]] > TimeIn[belong_to[b]];
    });
    
    int steps = 0, stars;
    // Trace the path from source -> gas -> star[1] -> gas -> star[2] -> ...
    //ofstream out("path.txt");
    int index_src = Index::encode(src);
    vector<bool> vis(m * n, 0);
    int last = index_src;
    auto From4rangersWithLove = [&](int index_src, vector<int>& setStar, bool skip_first_element = false) {
        while (setStar.size()) {
            int index_star = setStar.back();
            setStar.pop_back();
            if (vis[index_star]) continue;
            vector<int> path = Solve(index_src, index_star, belong_to, par, h, par_gas, h_gas);
            for (int i = 0; i < (int)path.size(); ++i) {
                int id = path[i];
                Point now = Index::decode(id);
                if (map[now.x][now.y] == STAR && !vis[id]) {
                    ++stars;
                    map[now.x][now.y] = STREET;
                }
                vis[id] = 1;
                Point pt = Index::decode(id);
                if (i == (int)path.size() - 1) {
                    if (setStar.empty()) {
                        if (!skip_first_element) {
                            ++steps;
                            cout << pt.x + 1 << ' ' << pt.y + 1 << '\n';
                            last = id;
                        }
                    }
                } else {
                    if (!skip_first_element) {
                        ++steps;
                        cout << pt.x + 1 << ' ' << pt.y + 1 << '\n';
                        last = id;
                        // if (!fuel) {
                        //     return id;
                        // }
                    }
                }
                skip_first_element = false;
            }
            index_src = index_star;
        }
        return index_src;
    };
    bool temp = (int)setStar[best_gas].size();
    index_src = From4rangersWithLove(index_src, setStar[best_gas]);
    if (temp) fprintf(stderr, "Hiiiii\n");
    //From4rangersWithLove(index_src, setCandidateStar[best_gas], temp);
    
    auto Greedy = [&](int src, vector<int>& setCandidateStar, vector<vector<int>>&map, int m, int n, int MAX_GAS) {
        while (true){
            queue<Point> q;
            vector<int> dist(m * n, -1), bef(m * n, -1);
            dist[src] = 0;
            Point pt = Index::decode(src);
            q.push(Index::decode(src));
            bool flag = false;
            fprintf(stderr, "Choose %d, %d\n, %d", pt.x, pt.y, MAX_GAS);
            while (!q.empty()) {
                Point u = q.front(); q.pop();
                int index_u = Index::encode(u);
                if (map[u.x][u.y] == STAR) {
                    vector<int> path;
                    int x = index_u;
                    while (x != src) {
                        path.push_back(x);
                        x = bef[x];
                    }
                    if (last != x) {
                        path.push_back(x);
                    }
                    reverse(path.begin(), path.end());
                    for (int x : path) {
                        Point pt = Index::decode(x);
                        cout << pt.x + 1 << ' ' << pt.y + 1 << '\n';
                        last = x;
                    }
                    flag = true;
                    MAX_GAS -= dist[index_u]; 
                    map[u.x][u.y] = STREET;
                    src = index_u;
                    break;
                }
                if (dist[index_u] == MAX_GAS) continue;
                ////fprintf(stderr,"At (%d, %d) with dist = %d:\n", u.x, u.y, dist[index_u]);
                for (int i = 0; i < 4; ++i) {
                    Point v = u.add_dir(i);
                    int index_v = Index::encode(v);
                    if (index_v != -1 && map[v.x][v.y] != WALL) {
                        if (dist[index_v] == -1) {
                            dist[index_v] = dist[index_u] + 1;
                            bef[index_v] = index_u;
                            q.push(v);
                        }
                    }
                }
            }
            if (!flag) return;
        }
        return; 
        
    };
    Greedy(index_src, setCandidateStar[best_gas], map, m, n, GAS_MAX);
    //cout << "Done\n";
    fprintf(stderr, "Total stars: %d, total steps: %d", stars, steps);
    auto t2 = std::chrono::steady_clock::now();
    auto d_milli = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    fprintf(stderr, "Total time %ld ms.\n", d_milli);
}
