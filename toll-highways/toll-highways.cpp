#include <algorithm>
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
    int64_t len() {
      return x * x + y * y;
    }
};

struct line {
    int64_t a, b, c;

    line() : a(), b(), c() {}
    line(int64_t a, int64_t b, int64_t c) : a(a), b(b), c(c) {}
    int64_t dist(point & p) {
        return a * p.x + b * p.y + c;
    }
};

point operator-(point & a, point & b) {
    return {a.x - b.x, a.y - b.y};
}

struct segment {
    point a;
    point b;
    
    segment(point & a, point & b) : a(a), b(b) {}
};

inline int64_t inner(point a, point b) {
    return a.x * b.x + a.y * b.y;
}

inline int64_t product(point a, point b) {
    return a.x * b.y - a.y * b.x;
}

inline int64_t dist2(point a, point b) {
    return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}

bool lineIntersectsSegment(line & l, segment & s) {
  int64_t da = l.dist(s.a), db = l.dist(s.b);
  return !((da > 0 && db > 0) || (da < 0 && db < 0));
}

int lineIntersectsPolygon(line & l, vector<point> & P, int k) {
    int sz = P.size();
    int n = (sz + k - 1) / k;
    int64_t minDist = numeric_limits<int64_t>::max();
    int minPoint = -1;

    if (n <= 6) {
        for (int i = k; i < sz; i += k) {
            segment s = {P[i - k], P[i]};
            if (lineIntersectsSegment(l, s)) {
	      return -1;
	    } else {
	      int64_t dist = abs(l.dist(P[i]));
	      if (minDist > dist) {
                minDist = dist;
                minPoint = i;
              }
	    }
        }
    } else {
        int i = lineIntersectsPolygon(l, P, 2 * k);
	if (i == -1) {
	  return -1;
	}
	point p1 = P[(i - k + sz) % sz], p2 = P[i], p3 = P[(i + k) % sz];
	segment s1 = {p1, p2}, s2 = {p2, p3};
	if (lineIntersectsSegment(l, s1) || lineIntersectsSegment(l, s2)) {
	  return -1;
	}
	for (int j = i - k; j <= i + k; j += k) {
            int64_t dist = abs(l.dist(P[(j + sz) % sz]));
	    if (minDist > dist) {
	        minDist = dist;
	        minPoint = j;
	    }
        }
    }
    return minPoint;
}


int main() {
    int ln, n;

    freopen("highways.in",  "r", stdin);
    freopen("highways.out", "w", stdout);

    scanf("%d%d", &ln, &n);
    vector<line> lines(ln);
    vector<point> pts(n);

    for (int i = 0; i < ln; i++) {
        scanf("%lld%lld%lld", &lines[i].a, &lines[i].b, &lines[i].c);
    }
    for (int i = 0; i < n; i++) {
        scanf("%lld%lld", &pts[i].x, &pts[i].y);
    }
    
    point rm = pts[0];
    for (int i = 1; i < n; i++) {
        if (rm.y < pts[i].y || (rm.y == pts[i].y && rm.x < pts[i].x)) {
            rm = pts[i];
        }
    }
    
    vector<point> hull(n + 1);
    int top = 0;
    hull[top++] = rm;
    sort(pts.begin(), pts.end(), [&rm](point & a, point & b) -> bool {
        int64_t tv = product(a - rm, b - rm);
        if (tv == 0) {
            return (a - rm).len() < (b - rm).len();
        } else {
            return tv > 0;
        }
    });
    for (int i = 0; i < n; i++) {
        if ((pts[i] - rm).len()) {
            hull[top++] = pts[i];
            break;
        }
    }
    for (int i = 0; i < n; i++) {
        if ((pts[i] - rm).len()) {
            while (top > 1 && product(hull[top - 1] - hull[top - 2], pts[i] - hull[top - 1]) <= 0) {
                top--;
            }
            hull[top++] = pts[i];
        }
    }
    hull[top] = hull[0];
    hull.resize(top + 1);

    vector<int> ans;
    for (int i = 0; i < ln; i++) {
        line & l = lines[i];
	if (lineIntersectsPolygon(l, hull, 1) == -1) {
	  ans.push_back(i);
	}
    }

    printf("%lu\n", ans.size());
    for (int a : ans) {
        printf("%d ", a + 1);
    }
    printf("\n");

    return 0;
}
