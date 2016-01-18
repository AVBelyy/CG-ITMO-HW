#include <cstddef>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

using namespace std;

const int64_t inf = 1e9 + 2015;

struct Point {
    int64_t x;
    int64_t y;

    Point() : x(0), y(0) {
    }

    Point(int64_t x, int64_t y) : x(x), y(y) {
    }
};

class Segment {
    Point a;
    Point b;

    enum Orientation {
        Left,
        Center,
        Right
    };

    static Orientation getOrientation(const Point & a, const Point & b, const Point & c) {
        int64_t V = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        if (V < 0) {
            return Left;
        } else if (V > 0) {
            return Right;
        } else {
            return Center;
        }
    }

    bool isBetween(const Point & c) const {
        int64_t x1 = min(a.x, b.x), x2 = max(a.x, b.x);
        int64_t y1 = min(a.y, b.y), y2 = max(a.y, b.y);
        return (x1 <= c.x && c.x <= x2) && (y1 <= c.y && c.y <= y2);
    }

    public:
    Segment(const Point & a, const Point & b) : a(a), b(b) {
    }
    
    bool contains(const Point & p) const {
        return getOrientation(a, b, p) == Center && isBetween(p);
    }

    bool intersects(const Segment & other) const {
        vector<Orientation> orientations = { getOrientation(a, b, other.a),       getOrientation(a, b, other.b),
                                             getOrientation(other.a, other.b, a), getOrientation(other.a, other.b, b) };
        if (orientations[0] != orientations[1]) {
            return orientations[2] != orientations[3];
        } else if (orientations[0] == Center) {
            if (a.x == b.x && a.y == b.y && orientations[2] != Center) {
                return false;
            } else {
                return isBetween(other.a) || isBetween(other.b) || other.isBetween(a) || other.isBetween(b);
            }
        } else {
            return false;
        }
    }
};

int main() {
    int64_t n, x0, y0;
    cin >> n >> x0 >> y0;
    Point p = {x0, y0};
    Segment ray = {p, {inf, y0 + 1}};

    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        Point p;
        cin >> p.x >> p.y;
        points[i] = p;
    }
    int count = 0;
    bool onBound = false;
    for (int i = 0; i < n; i++) {
        Segment s = {points[i], points[(i + 1) % n]};
        if (s.contains(p)) {
            onBound = true;
            break;
        }
        if (ray.intersects(s)) {
            count++;
        }
    }
    cout << (onBound || count % 2 == 1 ? "YES" : "NO") << endl;

    return 0;
}
