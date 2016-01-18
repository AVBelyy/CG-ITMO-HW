#include <iostream>
#include <utility>
#include <vector>
#include <limits>
#include <cmath>

using namespace std;

struct point {
    int64_t x;
    int64_t y;

    point() : x(0), y(0) {}
    point(int64_t x, int64_t y) : x(x), y(y) {}
};

point operator-(point & a, point & b) {
    return {a.x - b.x, a.y - b.y};
}

struct segment {
    point a;
    point b;
    
    segment(point & a, point & b) : a(a), b(b) {}
};

typedef vector<point> polygon;

inline int64_t inner(point a, point b) {
    return a.x * b.x + a.y * b.y;
}

inline int64_t dist2(point a, point b) {
    return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}

double pointToSegmentDist(point & p, segment & s) {
    int64_t len2 = dist2(s.a, s.b);
    if (len2 == 0) {
        return dist2(p, s.a);
    }
    int64_t t = inner(p - s.a, s.b - s.a);
    if (t < 0) {
        return dist2(p, s.a);
    } else if (t > len2) {
        return dist2(p, s.b);
    } else {
        double xp = s.a.x + ((double) t) * (s.b.x - s.a.x) / len2;
        double yp = s.a.y + ((double) t) * (s.b.y - s.a.y) / len2;
        return (p.x - xp) * (p.x - xp) + (p.y - yp) * (p.y - yp);
    }
}

pair<int, double> polygonToSegmentDist(polygon & P, int k, segment & s) {
    int sz = P.size();
    int n = (sz + k - 1) / k;
    double minDist = numeric_limits<double>::max();
    int minPoint = -1;

    if (n <= 4) {
        for (int i = 0; i < sz; i += k) {
            double dist = pointToSegmentDist(P[i], s);
            if (minDist > dist) {
                minDist = dist;
                minPoint = i;
            }
        }
    } else {
        int indMinPoint = polygonToSegmentDist(P, 2 * k, s).first;
        // check neighbors of minPoint in the original polygon
        for (int i = indMinPoint - k; i <= indMinPoint + k; i += k) {
            point & p = P[(i + sz) % sz];
            double dist = pointToSegmentDist(p, s);
            if (minDist > dist) {
                minDist = dist;
                minPoint = (i + sz) % sz;
            }
        }
    }

    return {minPoint, minDist};
}

double polygonDist(polygon & p1, polygon & p2) {
    int n1 = p1.size(), n2 = p2.size();
    double ans = numeric_limits<double>::max();

    for (int i = 0; i < n1; i++) {
        segment s = {p1[i], p1[(i + 1) % n1]};
        ans = min(ans, polygonToSegmentDist(p2, 1, s).second);
    }
    for (int i = 0; i < n2; i++) {
        segment s = {p2[i], p2[(i + 1) % n2]};
        ans = min(ans, polygonToSegmentDist(p1, 1, s).second);
    }

    return ans;
}

int main() {
    freopen("distance.in",  "r", stdin);
    freopen("distance.out", "w", stdout);

    polygon p1, p2;
    int n1, n2;
    cin >> n1;
    for (int i = 0; i < n1; i++) {
        point p;
        cin >> p.x >> p.y;
        p1.push_back(p);
    }
    cin >> n2;
    for (int i = 0; i < n2; i++) {
        point p;
        cin >> p.x >> p.y;
        p2.push_back(p);
    }

    printf("%.12lf\n", sqrt(polygonDist(p1, p2)));

    return 0;
}
