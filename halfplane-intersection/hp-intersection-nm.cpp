#include <unordered_set>
#include <functional>
#include <algorithm>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <utility>
#include <vector>
#include <limits>
#include <ctime>
#include <cmath>

using namespace std;

struct point {
    double x;
    double y;

    point() : x(), y() {}
    point(double x, double y) : x(x), y(y) {}
    double len() {
      return x * x + y * y;
    }
};

typedef vector<point> polygon;

const double eps = 1e-16;

struct line {
    int64_t a, b, c;

    line() : a(), b(), c() {}
    line(int64_t a, int64_t b, int64_t c) : a(a), b(b), c(c) {}
    double dist(point p) {
        return a * p.x + b * p.y + c;
    }
    bool contains(point p) {
        return dist(p) > eps;
    }
};

point operator*(point a, double t) {
    return {a.x * t, a.y * t};
}

point operator+(point a, point b) {
    return {a.x + b.x, a.y + b.y};
}

point operator-(point a, point b) {
    return {a.x - b.x, a.y - b.y};
}

bool operator==(point & a, point & b) {
    return fabs(a.x - b.x) < eps && fabs(a.y - b.y) < eps;
}

struct segment {
    point a;
    point b;
    
    segment(point a, point b) : a(a), b(b) {}
};

inline double inner(point a, point b) {
    return a.x * b.x + a.y * b.y;
}

inline double product(point a, point b) {
    return a.x * b.y - a.y * b.x;
}

inline double triangle(point p1, point p2, point p3) {
    return 0.5 * product(p1 - p3, p2 - p3);
}

inline double dist2(point a, point b) {
    return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}

bool lineIntersectsSegment(line & l, segment & s) {
  double da = l.dist(s.a), db = l.dist(s.b);
  return !((da > eps && db > eps) || (da < -eps && db < -eps));
}

pair<polygon, polygon> split(polygon & P, int k) {
    polygon p1(P.begin(), P.begin() + k);
    polygon p2(P.begin() + k, P.end());
    return {p1, p2};
}

typedef unordered_set<int> result;

result lineIntersectsPolygon(line & l, polygon & P) {
    result points;

    point prev = P[0];
    for (size_t i = 1; i < P.size(); i++) {
        point cur = P[i];
        segment s = {prev, cur};
        if (lineIntersectsSegment(l, s)) {
            points.insert(i - 1);
        }
        prev = cur;
    }

    return points;
}

point intersect(line & l, segment s) {
    double da = fabs(l.dist(s.a)), db = fabs(l.dist(s.b));
    if (da + db < eps) {
        return s.a;
    } else {
        double t = da / (da + db);
        return s.a + (s.b - s.a) * t;
    }
}

bool is_inf(double x) {
    return fabs(x - 1e9) < 1.0 || fabs(x + 1e9) < 1.0;
}

int main() {
    int ln;
    srand(time(NULL));

    freopen("area.in",  "r", stdin);
    freopen("area.out", "w", stdout);

    scanf("%d", &ln);
    vector<line> lines(ln);

    for (int i = 0; i < ln; i++) {
        scanf("%lld%lld%lld", &lines[i].a, &lines[i].b, &lines[i].c);
    }

    polygon P;
    bool empty = false;
    point searchP[] = {{1e9, 1e9}, {-1e9, 1e9}, {-1e9, -1e9}, {1e9, -1e9}, {1e9, 1e9}};
    for (int i = 0; i < 5; i++) {
        P.push_back(searchP[i]);
    }
    for (int i = 0; i < ln; i++) {
        line & L = lines[i];
        auto ans = lineIntersectsPolygon(L, P);
        vector<int> pts;
        for (int j : ans) {
            point p = P[j], q = P[j + 1];
            point x = intersect(L, {p, q});
            if (!(x == q)) {
                pts.push_back(j);
            }
        }
        if (pts.size() == 0) {
            if (L.contains(P[0])) {
                continue;
            } else {
                empty = true;
                break;
            }
        }
        if (pts.size() == 1) {
            if (L.contains(P[pts[0] + 1])) {
                continue;
            } else {
                empty = true;
                break;
            }
        }
        if (pts.size() == 2) {
            point o = P[0];
            int l = min(pts[0], pts[1]), r = max(pts[0], pts[1]);
            //printf("%d %d\n", l, r);
            point l1 = P[l], l2 = P[l + 1];
            point r1 = P[r], r2 = P[r + 1];
            point p = intersect(L, {l1, l2});
            point q = intersect(L, {r1, r2});
            // Check if line is collinear to one of the polygon's side
            if (p == l1 && q == l2) {
                if (L.contains(P[l + 2])) {
                    continue;
                } else {
                    empty = true;
                    break;
                }
            } else if (p == r1 && q == r2) {
                if (L.contains(P[r + 2])) {
                    continue;
                } else {
                    empty = true;
                    break;
                }
            }
            auto p1 = split(P, l + 1);
            auto p2 = split(p1.second, r - l);
            if (L.contains(o)) {
                polygon pq = {p, q};
                P = p1.first;
                P.insert(P.end(), pq.begin(), pq.end());
                P.insert(P.end(), p2.second.begin(), p2.second.end());
            } else {
                polygon qp = {q, p};
                P = p2.first;
                P.insert(P.end(), qp.begin(), qp.end());
                P.push_back(P[0]);
            }
        }
    }

    // Calculate area
    double area = 0.0;
    if (!empty) {
        point o = P[0];
        point a = o;
        for (size_t i = 1; i < P.size(); i++) {
            point b = P[i];
            if (is_inf(a.x) || is_inf(a.y) || is_inf(b.x) || is_inf(b.y)) {
                area = -1;
                break;
            } else if (i != 1 && i != P.size() - 1) {
                area += triangle(o, a, b);
            }
            a = b;
        }
    }

    if (area < 0) {
        printf("-1\n");
    } else {
        printf("%.10lf\n", area);
    }

    return 0;
}
