#include "define.hpp"

Point::Point(int x, int y):x(x), y(y) {}

void Point::read() {
    cin >> x >> y;
    --x, --y;
}

Point Point::add_dir(int i) {
    return Point(x + dir_row[i], y + dir_col[i]);
}
bool Point::operator==(const Point &a) {
    return x == a.x && y == a.y;
}

namespace Index{
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

vector<int> operator+(const vector<int>&a, const vector<int> &b) {
    vector<int> c;
    for (int x:a) c.push_back(x);
    for (int x:b) c.push_back(x);
    return c;
}

void operator+=(vector<int>&a, const vector<int> &b) {
    for (int x:b) a.push_back(x);
}