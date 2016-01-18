#include <cstddef>
#include "tests.h"
#include <iostream>
#include <gmpxx.h>
#include <vector>
#include <limits>
#include <cmath>

template <typename T>
struct Point {
    T x;
    T y;

    Point(T x, T y) : x(x), y(y) {
    }
};

template <typename T>
class Segment {
    Point<T> a;
    Point<T> b;

    enum Orientation {
        Left,
        Center,
        Right
    };

    static Orientation getOrientation(const Point<double> & a, const Point<double> & b, const Point<double> & c) {
        long double a1 = ((long double)(b.x - a.x)) * (c.y - a.y);
        long double a2 = ((long double)(b.y - a.y)) * (c.x - a.x);
        long double V = a1 - a2;
        long double eps = 8 * std::numeric_limits<double>::epsilon();
        if (V < -eps) {
            return Left;
        } else if (V > eps) {
            return Right;
        } else {
            // Higher precision we need here
            Point<mpq_class> ma(a.x, a.y);
            Point<mpq_class> mb(b.x, b.y);
            Point<mpq_class> mc(c.x, c.y);
            return getOrientation(ma, mb, mc);
        }
    }

    static Orientation getOrientation(const Point<mpq_class> & a, const Point<mpq_class> & b, const Point<mpq_class> & c) {
        mpq_class V = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        if (V < 0) {
            return Left;
        } else if (V > 0) {
            return Right;
        } else {
            return Center;
        }
    }

    bool isBetween(const Point<T> & c) const {
        double x1 = std::min(a.x, b.x);
        double x2 = std::max(a.x, b.x);
        double y1 = std::min(a.y, b.y);
        double y2 = std::max(a.y, b.y);
        return (x1 <= c.x && c.x <= x2) && (y1 <= c.y && c.y <= y2);
    }

    public:
    Segment(const Point<T> & a, const Point<T> & b) : a(a), b(b) {
    }

    bool intersects(const Segment<T> & other) const {
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
    int testN;
    std::cin >> testN;
    auto test = genTest(testN);
    for (int i = 0; i < test.size(); i += 8) {
        Segment<double> s1(Point<double>(test[i], test[i + 1]), Point<double>(test[i + 2], test[i + 3]));
        Segment<double> s2(Point<double>(test[i + 4], test[i + 5]), Point<double>(test[i + 6], test[i + 7]));
        std::cout << (s1.intersects(s2) ? "Y" : "N");
    }

    return 0;
}
