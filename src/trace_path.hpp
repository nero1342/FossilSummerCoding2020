#ifndef __TRACE_PATH__
#define __TRACE_PATH__
#include "define.hpp"

/*
Build DFS tree to make it easy to trace path and build dfs time_in(used to sort star-order to collect)
*/
void dfs(int u, const vector<vector<int>> &edge, vector<int>&par, vector<int>&h, vector<int>&TimeIn);

/*
Trace the path from src to dest in the DFS tree that has the infomation at vector<int> par and h.
There are 2 function here: 
    - Trace path from source/star to gas station or vice versa.
    - Trace path from station src to station dest in "Gas Station Spanning Tree".
*/
vector<int> Go(int src, int dest, const vector<int>& par, const vector<int>& h);

/*
Trace the path from (both may be source/star) src to dest by dividing to 3 parts:
-   From src to the nearest gas station of src.
-   From the nearest gas station of src to the nearest gas station of dest.
-   From the nearest gas station of dest to dest.
*/
vector<int> Solve(int src, int dest, const vector<int> &belong_to,
                const vector<int>&par, const vector<int>&h,
                const vector<int>&par_gas, const vector<int>&h_gas);

#endif // __TRACE_PATH__