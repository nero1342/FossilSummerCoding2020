#include <bits/stdc++.h>

using namespace std;

#define RM (RAND_MAX + 1)
#define RA (long) (rand())
long Rand(long l, long r) {
    long t = (RM * RM * RA + RM * RA + RA)  % (r - l + 1);
    t += (t < 0 ? r - l + 1 : 0);
    return t + l;
}
int main() {
    srand(time(NULL));
    int m = 50, n = 50, k = 6;
    int x = Rand(1, m), y = Rand(1, n);
    x = 1, y = 1;
    cout << x << ' ' << y << '\n' << k << '\n' << m << ' ' << n << '\n';
    int star = 0, gas = 0;
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (x == i && y == j) cout << 1 << ' ';
            else {
                int t = Rand(1, 100);
                if (t <= 10) cout << 0 << ' ';
                else if (t <= 96) cout << 1 << ' ';
                else if (t <= 98) cout << 2 << ' ', ++gas;
                else cout << 3 << ' ', ++star;
            }
        }
        cout << endl;
    }
    cerr << star << ' ' << gas << '\n';
}