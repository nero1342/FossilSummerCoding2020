
#ifndef __DEFINE__
#define __DEFINE__
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
    Point(int x = 0, int y = 0);
    void read();
    Point add_dir(int i);
    bool operator==(const Point &a);
};

namespace Index{
    extern int m, n;
    void init(int _m, int _n);
    int encode(Point u);
    Point decode(int x);
};


vector<int> operator+(const vector<int>&a, const vector<int> &b);
void operator+=(vector<int>&a, const vector<int> &b);

#endif // __DEFINE__