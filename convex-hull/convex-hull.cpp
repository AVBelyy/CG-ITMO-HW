#include <algorithm>
#include <cstdio>
#include <vector>
#include <cmath>

using namespace std; 

struct point {
    long long x, y;

    point() : x(0), y(0) {}
    point(long long x, long long y) : x(x), y(y) {}
};

vector<point> pts;
vector<point> hull;

long long turn(point a, point b, point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool sorter(point a, point b) {
    if (a.x == 0 && b.x == 0) {
        return b.y < a.y;
    } else if (a.x == 0 || b.x == 0) {
        return b.x < a.x;
    }
    long double yxa = ((long double) a.y) / ((long double)a.x);
    long double yxb = ((long double) b.y) / ((long double)b.x);
    if (fabs(yxa - yxb) < 1e-12) {
        return a.x*a.x + a.y*a.y < b.x*b.x + b.y*b.y;
    } else {
        return yxa < yxb;
    }
}

void preparePoints() {
    // find leftmost point
    point lm = pts[0];
    for (point p : pts) {
        if (lm.x == p.x) {
            if (lm.y > p.y) {
                lm = p;
            }
        } else if (lm.x > p.x) {
            lm = p;
        }
    }

    // filter points
    vector<point> pts2;
    for (point p : pts) {
        if (!(p.x == lm.x && p.y == lm.y)) {
            pts2.push_back(point(p.x - lm.x, p.y - lm.y));
        }
    }

    // sort points
    sort(pts2.begin(), pts2.end(), sorter);
    pts = {lm};
    for (point p : pts2) {
        pts.push_back(point(p.x + lm.x, p.y + lm.y));
    }
}

void convexHull() {
    hull.push_back(pts[0]);
    hull.push_back(pts[1]);
    pts.push_back(pts[0]);

    size_t n = pts.size();
    for (int i = 2; i < n; i++) {
        point nxtop = hull[hull.size() - 2];
        point top = hull[hull.size() - 1];
        long long tv = turn(nxtop, top, pts[i]);
        if(tv < 0) {
            hull.pop_back();
            i--;
        } else if (tv == 0) {
            hull.pop_back();
            hull.push_back(pts[i]);
        } else {
            hull.push_back(pts[i]);
        }
    }
}

int main() {
    int n;

    scanf("%d", &n);
    pts.resize(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        scanf("%lld%lld", &pts[i].x, &pts[i].y);
    }

    preparePoints();
    convexHull();

    int m = hull.size();
    printf("%d\n", m - 1);
    for (int i = 1; i < m; i++) {
        printf("%lld %lld\n", hull[i].x, hull[i].y);
    }

    return 0;
}
