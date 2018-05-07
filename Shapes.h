#ifndef KOSTOV_OOP_SHAPES_H
#define KOSTOV_OOP_SHAPES_H

#include <cmath>
#include <sstream>
#include <stdexcept>
#include "Base.h"
#include "Container.h"

using namespace std;


#define EPSILON 0.000001
inline bool eq(double a, double b) {
    return abs(a - b) <= EPSILON;
}
inline double sq(double a) {
    return a*a;
}


class Shape : public Printable {
public:
    Shape() {
        count++;
    }
    Shape(const Shape& s) {
        count++;
    }

    virtual ~Shape() {
        count--;
    }

    friend ostream& operator<<(ostream& os, const Shape& shape) {
        return os << shape.print();
    }

    static int GetCount() {
        return count;
    }

private:
    static unsigned int count;
};


class Point : public Shape, public Named {
public:
    Point(double x, double y) : Named(string("Point")), x(x), y(y) {}
    Point(const Point& p) = default;

    const string print() const override {
        stringstream s;
        s << "Point(" << x << ", " << y << ")";
        return s.str();
    }

    bool operator==(const Point& p) const {
        return eq(x, p.x) && eq(y, p.y);
    }
    bool operator!=(const Point& p) const {
        return !(*this == p);
    }
    Point& operator=(const Point& p) {
        x = p.x;
        y = p.y;
        return *this;
    }

    double x;
    double y;
};


class Circle : public Shape, public Named {
public:
    Circle(const Point& c, double r) : Named(string("Circle")), c(c), r(r) {
        if (r <= 0) throw std::invalid_argument("Circle must have positive radius.");
    }
    Circle(const Circle& c) = default;

    const string print() const override {
        stringstream s;
        s << "Circle(" << c << ", r = " << r << ", area = " << area() << ")";
        return s.str();
    }

    double area() const {
        return M_PI * r * r;
    }

    const Point c;
    const double r;
};


class Rect : public Shape, public Named {
protected:
    Rect(const Point& a, const Point& b, const string& name) : Named(name), a(a), b(b) {
        if (eq(a.x, b.x) || eq(a.y, b.y)) throw std::invalid_argument("Rectangle must not be a line or dot");
    }

public:
    Rect(const Point& a, const Point& b) : Rect(a, b, string("Rect")) {}
    Rect(const Rect& r) = default;

    const string print() const override {
        stringstream s;
        s << name << "(" << a << ", " << b << ", area = " << area() << ")";
        return s.str();
    }

    double area() const {
        return abs((a.x - b.x) * (a.y - b.y));
    }

    const Point a;
    const Point b;
};


class Square : public Rect {
public:
    Square(const Point& a, const Point& b) : Rect(a, b, string("Square")) {
        if (!eq(abs(a.x - b.x), abs(a.y - b.y))) throw std::invalid_argument("Square must have equal sides");
    }
    Square(const Square& s) = default;
};


class Polyline : public Shape, public Named {
protected:
    Polyline(const string& name) : Named(name) {}

public:
    Polyline() : Polyline(string("Polyline")) {}
    Polyline(const Polyline& p) = default;

    virtual Polyline& addPoint(const Point& p) {
        points.addLast(p);
        return *this;
    }

    const string print() const override {
        if (points.getSize() < 2) throw VertexUnderflow();

        stringstream s;
        s << name << "(";
        points.forEach(
                [&] (const Point& p) { s << p << ", "; }
        );
        s << "length = " << length() << ")";
        return s.str();
    }

    virtual double length() const {
        if (points.getSize() < 2) throw VertexUnderflow();

        double len = 0;
        Point prev = points.getFirst();

        points.forEach(
                [&] (const Point& cur) {
                    len += sqrt( sq(cur.x - prev.x) + sq(cur.y - prev.y) );
                    prev = cur;
                },
                1 // skip first elem
        );

        return len;
    }


    class VertexUnderflow : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Polyline must contain at least 2 vertices.";
        }
    };

protected:
    Container<Point> points;
};


class Polygon : public Polyline {
public:
    Polygon() : Polyline(string("Polygon")), complete(false) {}
    Polygon(const Polygon& p) = default;

    Polyline& addPoint(const Point& p) override {
        if (complete) throw Complete();

        if (!points.isEmpty() && p == points.getFirst()) {
            if (points.getSize() < 3) throw VertexUnderflow();
            complete = true;
        }
        return Polyline::addPoint(p);
    }

    double length() const override {
        if (!complete) throw Incomplete();
        return Polyline::length();
    }

    double perimeter() const {
        return length();
    }

    const string print() const override {
        if (!complete) throw Incomplete();
        string s = Polyline::print();
        size_t i = s.find("length");
        return s.replace(i, 6, " perim");
    }


    class Incomplete : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Polygon border is not connected.";
        }
    };
    class Complete : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Polygon border is connected, but you tried to add another point.";
        }
    };
    class VertexUnderflow : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Polygon must contain at least 3 vertices.";
        }
    };

private:
    bool complete;
};

#endif //KOSTOV_OOP_SHAPES_H
