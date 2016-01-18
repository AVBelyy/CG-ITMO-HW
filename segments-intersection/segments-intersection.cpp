#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <set>

using namespace std;

const double eps = 1e-9;

struct point {
    double x, y;

    point() : x(0), y(0) {}
    point(int x, int y) : x(x), y(y) {}
};
 
struct segment {
	point p, q;
	int id;
 
	double get_y(double x) const {
		if (fabs(p.x - q.x) < eps)  return p.y;
		return p.y + (q.y - p.y) * (x - p.x) / (q.x - p.x);
	}
};

inline bool intersect1d(double l1, double r1, double l2, double r2) {
	if (l1 > r1) {
        swap(l1, r1);
    }
	if (l2 > r2) {
        swap(l2, r2);
    }
	return max(l1, l2) <= min(r1, r2) + eps;
}
 
inline int turn(const point & a, const point & b, const point & c) {
	double s = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return s < -eps ? -1 : s > eps ? 1 : 0;
}
 
bool intersect(const segment & a, const segment & b) {
	return intersect1d(a.p.x, a.q.x, b.p.x, b.q.x)
		&& intersect1d(a.p.y, a.q.y, b.p.y, b.q.y)
		&& turn(a.p, a.q, b.p) * turn(a.p, a.q, b.q) <= 0
		&& turn(b.p, b.q, a.p) * turn(b.p, b.q, a.q) <= 0;
}
 
bool operator<(const segment & a, const segment & b) {
	double x = max(min(a.p.x, a.q.x), min(b.p.x, b.q.x));
	return a.get_y(x) < b.get_y(x) - eps;
}
 
struct event {
	double x;
	int tp, id;
 
	event() {}
	event(double x, int tp, int id) : x(x), tp(tp), id(id) {}
 
	bool operator<(const event & e) const {
		if (fabs(x - e.x) > eps) return x < e.x;
		return tp > e.tp;
	}
};
 
set<segment> s;
vector<set<segment>::iterator> where;
 
inline set<segment>::iterator prev(set<segment>::iterator it) {
	return it == s.begin() ? s.end() : --it;
}
 
inline set<segment>::iterator next(set<segment>::iterator it) {
	return ++it;
}
 
pair<int, int> solve(const vector<segment> & a) {
	int n = a.size();
	vector<event> e;
	for (int i = 0; i < n; i++) {
		e.emplace_back(min(a[i].p.x, a[i].q.x), +1, i);
		e.emplace_back(max(a[i].p.x, a[i].q.x), -1, i);
	}
	sort(e.begin(), e.end());
 
	s.clear();
	where.resize(a.size());
	for (size_t i = 0; i < e.size(); i++) {
		int id = e[i].id;
		if (e[i].tp == 1) {
			auto nxt = s.lower_bound(a[id]), prv = prev(nxt);
			if (nxt != s.end() && intersect (*nxt, a[id])) {
				return {nxt->id, id};
            }
			if (prv != s.end() && intersect (*prv, a[id])) {
				return {prv->id, id};
            }
			where[id] = s.insert(nxt, a[id]);
		} else {
			auto nxt = next(where[id]), prv = prev(where[id]);
			if (nxt != s.end() && prv != s.end() && intersect(*nxt, *prv)) {
				return {prv->id, nxt->id};
            }
			s.erase(where[id]);
		}
	}
 
	return {-1, -1};
}

int main() {
    freopen("segments.in",  "r", stdin);
    freopen("segments.out", "w", stdout);

    int N;
    scanf("%d", &N);
    vector<segment> s(N);

    for (int i = 0; i < N; i++) {
        int x1, y1, x2, y2;
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        point p1 = {x1, y1}, p2 = {x2, y2};
        s[i] = {p1, p2, i};
    }

    auto segs = solve(s);

    if (segs.first == -1) {
        printf("NO\n");
    } else {
        printf("YES\n%d %d\n", segs.first + 1, segs.second + 1);
    }

    return 0;
}
