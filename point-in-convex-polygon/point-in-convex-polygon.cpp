#include <algorithm>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <cmath>

using namespace std;

struct point {
    int64_t x;
    int64_t y;
};

enum position {
    INSIDE,
    BORDER,
    OUTSIDE
};

vector<point> points;

// < 0 -- left
// > 0 -- right
int turn(point & a, point & b, point & c) {
    int64_t val = (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
    return val > 0 ? 1 : (val < 0 ? -1 : 0);
}

bool is_in_fan(point & a, point & b, point & c, point & p) {
    return turn(b, a, p) >= 0 && turn(b, c, p) < 0;
}

bool is_on_segment(point & a, point & b, point & p) {
    if (turn(a, b, p) != 0) {
        return false;
    }
    int64_t x1 = min(a.x, b.x), x2 = max(a.x, b.x);
    int64_t y1 = min(a.y, b.y), y2 = max(a.y, b.y);
    return x1 <= p.x && p.x <= x2 && y1 <= p.y && p.y <= y2;
}

position get_position(point & p) {
    int l = 1, r = points.size() - 1;
    point & b = points[0];
    if (is_on_segment(b, points[l], p) || is_on_segment(b, points[r], p)) {
        return BORDER;
    }
    if (!is_in_fan(points[l], b, points[r], p)) {
        return OUTSIDE;
    }
    while (l + 1 < r) {
        int m = (l + r) / 2;
        if (is_in_fan(points[l], b, points[m], p)) {
            r = m;
        } else if(is_in_fan(points[m], b, points[r], p)) {
            l = m;
        } else {
            exit(1);
        }
    }
    int dir = turn(points[l], points[r], p);
    if (dir == 0) {
        return BORDER;
    } else if (dir == 1) {
        return INSIDE;
    } else {
        return OUTSIDE;
    }
}

int main() {
    int n, k;

    freopen("inside.in",  "r", stdin);
    freopen("inside.out", "w", stdout);

    cin >> n;
    points.resize(n);
    for (int i = 0; i < n; i++) {
        point p;
        cin >> p.x >> p.y;
        points[i] = p;
    }

    // Check if sorting is clockwise
    if (turn(points[0], points[1], points[2]) < 0) {
        reverse(points.begin(), points.end());
    }

    cin >> k;
    for (int i = 0; i < k; i++) {
        point p;
        cin >> p.x >> p.y;
        auto pos = get_position(p);
        if (pos == INSIDE) {
            cout << "INSIDE" << endl;
        } else if (pos == BORDER) {
            cout << "BORDER" << endl;
        } else if (pos == OUTSIDE) {
            cout << "OUTSIDE" << endl;
        }
    }

    return 0;
}
