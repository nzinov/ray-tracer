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
        return sqrt(sq(x) + sq(y) + sq(z));
    }

    bool operator==(const Vector& other) {
        return fabs(x- other.x) < EPS && fabs(y - other.y) < EPS && fabs(z - other.z) < EPS;
    }

    Vector& operator+=(const Vector& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector& operator-=(const Vector& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector& operator*=(double factor) {
        x *= factor;
        y *= factor;
        z *= factor;
        return *this;
    }

    Vector& operator/=(double factor) {
        x /= factor;
        y /= factor;
        z /= factor;
        return *this;
    }

    Vector normalized() const {
        return *this / length();
    }

    friend Vector operator*(Vector a, Vector b) {
        return Vector(a.x*b.x, a.y*b.y, a.z*b.z);
    }

    friend double dot(Vector a, Vector b) {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    friend Vector vec(Vector a, Vector b) {
        return Vector(a.y*b.z-a.z*b.y, -(a.x*b.z-a.z*b.x), a.x*b.y-a.y*b.x);
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
