#ifndef __DSU__
#define __DSU__
#include "define.hpp"

struct DisjointSetUnion{
    vector<int> f;
    DisjointSetUnion(int n);
    int root(int u);
    bool Union(int u, int v);
    /*
    Merge the gas station in the same set to a vector.
    Return: the vector of the set above.
    */
    vector<vector<int>> convert2vector(const vector<Point>& mapping);
};
#endif //__DSU__