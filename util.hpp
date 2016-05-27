#ifndef UTIL_H
#define UTIL_H
#include <cmath>
#include <cassert>
#include <boost/operators.hpp>

template<class T> class Maybe {
    T value;
    bool has_value;
public:
    Maybe() : value(), has_value(false) {}
    Maybe(T val) : value(val), has_value(true) {}

    operator bool() {
        return has_value;
    }

    T val() {
        assert(has_value);
        return value;
    }
};

void log(const char* s) {
    printf("%s\n", s);
    fflush(stdout);
}

inline double sq(double val) {
    return val*val;
}

const double EPS = 1e-6;

inline bool almost_zero(double val) {
    return fabs(val) < EPS;
}

struct Vector : boost::additive<Vector>, boost::multiplicative2<Vector, double>, boost::equality_comparable<Vector> {
    double coord[3];
    Vector() : coord{0, 0, 0} {}
    Vector(double x, double y, double z) : coord{x, y, z} {}

    double length() const {
        return sqrt(sq(coord[0]) + sq(coord[1]) + sq(coord[2]));
    }

    bool operator==(const Vector& other) {
        for (int i = 0; i < 3; ++i) {
            if (fabs(coord[i] - other.coord[i]) > EPS) {
                return false;
            }
        }
        return true;
    }

    Vector& operator+=(const Vector& other) {
        for (int i = 0; i < 3; ++i) {
            coord[i] += other.coord[i];
        }
        return *this;
    }

    Vector& operator-=(const Vector& other) {
        for (int i = 0; i < 3; ++i) {
            coord[i] -= other.coord[i];
        }
        return *this;
    }

    Vector& operator*=(double factor) {
        for (int i = 0; i < 3; ++i) {
            coord[i] *= factor;
        }
        return *this;
    }

    Vector& operator/=(double factor) {
        for (int i = 0; i < 3; ++i) {
            coord[i] /= factor;
        }
        return *this;
    }

    Vector normalized() const {
        return *this / length();
    }

    friend Vector operator*(Vector a, Vector b) {
        return Vector(a.coord[0]*b.coord[0], a.coord[1]*b.coord[1], a.coord[2]*b.coord[2]);
    }

    friend double dot(Vector a, Vector b) {
        double ans = 0;
        for (int i = 0; i < 3; ++i) {
            ans += a.coord[i]*b.coord[i];
        }
        return ans;
    }

    friend Vector vec(Vector a, Vector b) {
        return Vector(a.coord[1]*b.coord[2]-a.coord[2]*b.coord[1], -(a.coord[0]*b.coord[2]-a.coord[2]*b.coord[0]), a.coord[0]*b.coord[1]-a.coord[1]*b.coord[0]);
    }

    friend double angle(Vector a, Vector b) {
        return dot(a, b) / a.length() / b.length();
    }
};

inline double sq(Vector v) {
    return dot(v, v);
}

typedef Vector Point;

#endif
