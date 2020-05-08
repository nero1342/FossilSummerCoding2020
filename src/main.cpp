#include "define.hpp"
#include "merge_gas_station.hpp"
#include "trace_path.hpp"

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

    // Merge gas station
    vector<vector<int>> setGasStation = merge_set_gas_station(map, m, n, GAS_MAX);
    fprintf(stderr,"There are %d set of groups of gas stations.\n", (int)setGasStation.size());
    for (int i = 0; i < (int) setGasStation.size(); ++i) {
        fprintf(stderr,"Set %d has %d gas stations:\n", i, (int)setGasStation[i].size());
        for (int index: setGasStation[i]) {
            Point u = Index::decode(index);
            fprintf(stderr,"(%d, %d), ", u.x, u.y);
        }
        fprintf(stderr,"\n");
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
        fprintf(stderr,"There is no gas station, need another solution.\n");
        return 0;
    }
    fprintf(stderr,"*Choose gas station %d\n", best_gas);

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
    
    
    // Trace the path from source -> gas -> star[1] -> gas -> star[2] -> ...
    //ofstream out("path.txt");
    int index_src = Index::encode(src);

    auto From4rangersWithLove = [&](int index_src, vector<int>& setStar, bool skip_first_element = false) {
        while (setStar.size()) {
            int index_star = setStar.back();
            setStar.pop_back();
            vector<int> path = Solve(index_src, index_star, belong_to, par, h, par_gas, h_gas);
            for (int i = 0; i < (int)path.size(); ++i) {
                int id = path[i];
                Point pt = Index::decode(id);
                if (i == (int)path.size() - 1) {
                    if (setStar.empty()) {
                        if (!skip_first_element) 
                            cout << pt.x + 1 << ' ' << pt.y + 1 << '\n';
                    }
                } else {
                    if (!skip_first_element) 
                        cout << pt.x + 1 << ' ' << pt.y + 1 << '\n';
                }
                skip_first_element = false;
            }
            index_src = index_star;
        }
        return index_src;
    };
    index_src = From4rangersWithLove(index_src, setStar[best_gas]);
    From4rangersWithLove(index_src, setCandidateStar[best_gas], true);
}
