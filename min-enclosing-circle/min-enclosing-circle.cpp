#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

struct point {
  long double x, y;

  point() : x(), y() {}
  point(long double x, long double y) : x(x), y(y) {}
};

point operator-(point & a, point & b) {
  return {a.x - b.x, a.y - b.y};
}

struct circle {
  point c;
  long double r2;
};

inline long double norm2(point & p) {
  return p.x * p.x + p.y * p.y;
}

inline long double dist2(point & p1, point & p2) {
  point p = p1 - p2;
  return norm2(p);
}

inline bool in_circle(circle & s, point & p) {
  return dist2(s.c, p) <= s.r2;
}

void make_circle_2p(circle & ans, point & p1, point & p2) {
  ans.c = {(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
  ans.r2 = dist2(ans.c, p1);
}

void make_circle_3p(circle & ans, point & a, point & b, point & c) {
  long double na = norm2(a), nb = norm2(b), nc = norm2(c);
  long double M11 = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
  long double M12 = na  * (b.y - c.y) + nb  * (c.y - a.y) + nc  * (a.y - b.y);
  long double M13 = na  * (b.x - c.x) + nb  * (c.x - a.x) + nc  * (a.x - b.x);
  ans.c = {M12 / M11 / 2, -M13 / M11 / 2};
  ans.r2 = dist2(ans.c, a);
}

void make_circle_with_2p(circle & ans, vector<point> & p, size_t count, point & q2) {
  point & q1 = p[count];
  make_circle_2p(ans, q1, q2);
  for (size_t i = 0; i < count; i++) {
    if (!in_circle(ans, p[i])) {
      make_circle_3p(ans, p[i], q1, q2);
    }
  }
}

void make_circle_with_p(circle & ans, vector<point> & p, size_t count) {
  point & q = p[count];
  random_shuffle(p.begin(), p.begin() + count);
  make_circle_2p(ans, p[0], q);
  for (size_t i = 1; i < count; i++) {
    if (!in_circle(ans, p[i])) {
      make_circle_with_2p(ans, p, i, q);
    }
  }
}

void make_circle(circle & ans, vector<point> & p) {
  random_shuffle(p.begin(), p.end());
  if (p.size() == 1) {
    ans.c = p[0];
    ans.r2 = 0;
  } else {
    make_circle_2p(ans, p[0], p[1]);
    for (size_t i = 2; i < p.size(); i++) {
      if (!in_circle(ans, p[i])) {
	make_circle_with_p(ans, p, i);
      }
    }
  }
}

int main() {
    int n;

    freopen("tower.in",  "r", stdin);
    freopen("tower.out", "w", stdout);

    scanf("%d", &n);
    vector<point> p(n);
    for (int i = 0; i < n; i++) {
      scanf("%Lf%Lf", &p[i].x, &p[i].y);
    }

    circle ans;
    make_circle(ans, p);
    printf("%Lf\n%Lf %Lf\n", sqrt(ans.r2), ans.c.x, ans.c.y);

    return 0;
}
