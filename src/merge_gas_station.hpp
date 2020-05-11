#ifndef __MERGE_GAS_STATION__
#define __MERGE_GAS_STATION__
#include "define.hpp"

/*
Define A is the set(gas_station x) satisfy \forall x \exist y, dist(x, y) <= GAS_MAX
Return: Vector of A;
*/
vector<vector<int>> merge_set_gas_station(const vector<vector<int>> &map, int m, int n, int GAS_MAX);

/*
With the group gas station id has set_gas, find out all stars that can be reach by gas station in set_gas.
So the distance from each star to group must be at least 
GAS_MAX / 2 (gas -> star -> gas is still smaller than GAS_MAX)
Check whether this group can reach the src or not (If not, we don't need to care about it)
*/
pair<vector<int>, vector<int>> bfs_group_gas_station_to_star(int id, const vector<int> &set_gas, 
            const vector<vector<int>>&map, int m, int n, int GAS_MAX, 
            bool &reach_src, Point src);

/*
Build Spanning Tree of gas station and save to graph_gas
Build the nearest gas station of each cell and save to belong_to
Return: the graph that connect source, stars, cells, and gas station. 
*/
vector<vector<int>> build_graph_gas_and_star(const vector<int>& setGasStation, 
        const vector<vector<int>>&map, int m, int n, int GAS_MAX, 
        vector<int> &belong_to, vector<vector<int>>& graph_gas);

#endif // __MERGE_GAS_STATION__