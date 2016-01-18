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

#define getSize(t) ((t) != NULL ? (t)->size : 0)

template <typename T>
class Treap {
    public:
        int priority;
        int size;
        T value;
        Treap<T> *parent;
        Treap<T> *left;
        Treap<T> *right;

        Treap(T);
        T get(int key);
        static Treap<T> *merge(Treap*, Treap*);
        static pair<Treap<T>*, Treap<T>*> split(Treap<T>*, int, int);
        static Treap<T> *traverse(Treap<T>*, int, int);
};

template <typename T>
Treap<T>::Treap(T value) : priority(rand()), size(1), value(value), parent(), left(), right() {}

template <typename T>
T Treap<T>::get(int key) {
    return traverse(this, (key + size) % size, 0)->value;
}

template <typename T>
Treap<T> *Treap<T>::merge(Treap<T> *t1, Treap<T> *t2) {
    Treap *t;
    if (!t1 || !t2) {
        t = t1 ? t1 : t2;
    } else if (t1->priority > t2->priority) {
        t1->right = merge(t1->right, t2);
        t = t1;
    } else {
        t2->left = merge(t1, t2->left);
        t = t2;
    }
    t->size = getSize(t->left) + getSize(t->right) + 1;
    return t;
}

template <typename T>
pair<Treap<T>*, Treap<T>*> Treap<T>::split(Treap<T> *t, int key, int add) {
    Treap *t1 = NULL, *t2 = NULL;
    if (t != NULL) {
        int curKey = add + getSize(t->left);
        if (key <= curKey) {
            pair<Treap*, Treap*> p = split(t->left, key, add);
            t1 = p.first;
            t->left = p.second;
            t2 = t;
        } else {
            pair<Treap*, Treap*> p = split(t->right, key, add + getSize(t->left) + 1);
            t->right = p.first;
            t2 = p.second;
            t1 = t;
        }
        t->size = getSize(t->left) + getSize(t->right) + 1;
    }
    return pair<Treap*, Treap*>(t1, t2);
}

template <typename T>
inline Treap<T> *Treap<T>::traverse(Treap<T> *t, int key, int cur) {
    int idx = cur + getSize(t->left);
    if (key == idx) {
        return t;
    } else if (key < idx) {
        return traverse(t->left, key, cur);
    } else {
        return traverse(t->right, key, idx + 1);
    }
}

struct point {
    double x;
    double y;

    point() : x(), y() {}
    point(double x, double y) : x(x), y(y) {}
    double len() {
      return x * x + y * y;
    }
};

typedef Treap<point> * polygon;

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

struct result {
    unordered_set<int> points;
    int closest;

    result(unordered_set<int> & points, int closest) : points(points), closest(closest) {}
};

result lineIntersectsPolygon(line & l, polygon & P, size_t sz, int k) {
    int n = (sz + k - 1) / k;
    double minDist = numeric_limits<double>::max();
    int minPoint = -1;
    unordered_set<int> points;

    if (n <= 50) {
        point prev = P->get(0);
        for (size_t i = k; i < sz; i += k) {
            point cur = P->get(i);
            segment s = {prev, cur};
            if (lineIntersectsSegment(l, s)) {
	            points.insert(i - k);
    	    } else {
	            double dist = fabs(l.dist(cur));
	            if (minDist > dist) {
                    minDist = dist;
                    minPoint = i;
                }
	        }
            prev = cur;
        }
    } else {
        auto ind = lineIntersectsPolygon(l, P, sz, 2 * k);
        if (ind.points.size() != 0) {
            for (int i : ind.points) {
                point p1 = P->get(i - k);
                point p2 = P->get(i);
                segment s = {p1, p2};
                if (lineIntersectsSegment(l, s)) {
                    points.insert((i - k + sz) % sz);
                } else {
                    points.insert(i);
                }
            }
        } else {
            int i = ind.closest;
            point p1 = P->get(i - 2 * k);
            point p2 = P->get(i - k);
            point p3 = P->get(i);
            point p4 = P->get(i + k);
            point p5 = P->get(i + 2 * k);
            point pts[] = {p1, p2, p3, p4, p5};
            segment segs[] = {{p1, p2}, {p2, p3}, {p3, p4}, {p4, p5}};
            for (int j = 0; j < 4; j++) {
                if (lineIntersectsSegment(l, segs[j])) {
                    points.insert((i + k * (j - 2) + sz) % sz);
                }
            }
            for (int j = 0; j < 5; j++) {
                double dist = fabs(l.dist(pts[j]));
                if (minDist > dist) {
                    minDist = dist;
                    minPoint = (i + k * (j - 2) + sz) % sz;
                }
            }
        }
    }

    return {points, minPoint};
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

    polygon P = NULL;
    bool empty = false;
    point searchP[] = {{1e9, 1e9}, {-1e9, 1e9}, {-1e9, -1e9}, {1e9, -1e9}, {1e9, 1e9}};
    for (int i = 0; i < 5; i++) {
        polygon Q = new Treap<point>(searchP[i]);
        P = Treap<point>::merge(P, Q);
    }
    for (int i = 0; i < ln; i++) {
        line & L = lines[i];
        auto ans = lineIntersectsPolygon(L, P, P->size, 1);
        vector<int> pts;
        for (int j : ans.points) {
            point p = P->get(j), q = P->get(j + 1);
            point x = intersect(L, {p, q});
            if (!(x == q)) {
                pts.push_back(j);
            }
        }
        if (pts.size() == 0) {
            if (L.contains(P->get(0))) {
                continue;
            } else {
                empty = true;
                break;
            }
        }
        if (pts.size() == 1) {
            if (L.contains(P->get(pts[0] + 1))) {
                continue;
            } else {
                empty = true;
                break;
            }
        }
        if (pts.size() == 2) {
            point o = P->get(0);
            int l = min(pts[0], pts[1]), r = max(pts[0], pts[1]);
            //printf("%d %d\n", l, r);
            point l1 = P->get(l), l2 = P->get(l + 1);
            point r1 = P->get(r), r2 = P->get(r + 1);
            point p = intersect(L, {l1, l2});
            point q = intersect(L, {r1, r2});
            // Check if line is collinear to one of the polygon's side
            if (p == l1 && q == l2) {
                if (L.contains(P->get(l + 2))) {
                    continue;
                } else {
                    empty = true;
                    break;
                }
            } else if (p == r1 && q == r2) {
                if (L.contains(P->get(r + 2))) {
                    continue;
                } else {
                    empty = true;
                    break;
                }
            }
            auto p1 = Treap<point>::split(P, l + 1, 0);
            auto p2 = Treap<point>::split(p1.second, r - l, 0);
            if (L.contains(o)) {
                auto pq = Treap<point>::merge(new Treap<point>(p), new Treap<point>(q));
                P = Treap<point>::merge(p1.first, pq);
                P = Treap<point>::merge(P, p2.second);
            } else {
                auto qp = Treap<point>::merge(new Treap<point>(q), new Treap<point>(p));
                P = Treap<point>::merge(p2.first, qp);
                P = Treap<point>::merge(P, new Treap<point>(P->get(0)));
            }
        }
    }

    // Calculate area
    double area = 0.0;
    if (!empty) {
        point o = P->get(0);
        point a = o;
        for (int i = 1; i < P->size; i++) {
            point b = P->get(i);
            if (is_inf(a.x) || is_inf(a.y) || is_inf(b.x) || is_inf(b.y)) {
                area = -1;
                break;
            } else if (i != 1 && i != P->size - 1) {
                area += triangle(o, a, b);
            }
            a = b;
        }
    }

    /*printf("Polygon size: %d\n", P->size);
    for (int i = 0; i < P->size; i++) {
        point p = P->get(i);
        printf("%lf %lf\n", p.x, p.y);
    }*/
    if (area < 0) {
        printf("-1\n");
    } else {
        printf("%.10lf\n", area);
    }

    return 0;
}
