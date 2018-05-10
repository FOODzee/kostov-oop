#pragma once

#include <cmath>
#include <sstream>
#include <stdexcept>
#include "Base.h"
#include "Container.h"


#define EPSILON 0.000001
inline bool eq(double a, double b) {
    return fabs(a - b) <= EPSILON;
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

    friend std::ostream& operator<<(std::ostream& os, const Shape& shape) {
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
    Point(double x, double y) : Named("Point"), x(x), y(y) {}
    Point(const Point& p) = default;

    const std::string print() const override {
        std::stringstream s;
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
    Circle(const Point& c, double r) : Named("Circle"), c(c), r(r) {
        if (r <= 0) throw std::invalid_argument("Circle must have positive radius.");
    }
    Circle(const Circle& c) = default;

    const std::string print() const override {
        std::stringstream s;
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
    Rect(const Point& a, const Point& b, const char* name) : Named(name), a(a), b(b) {
        if (eq(a.x, b.x) || eq(a.y, b.y)) throw std::invalid_argument("Rectangle must not be a line or dot");
    }

public:
    Rect(const Point& a, const Point& b) : Rect(a, b, "Rect") {}
    Rect(const Rect& r) = default;

    const std::string print() const override {
        std::stringstream s;
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
    Square(const Point& a, const Point& b) : Rect(a, b, "Square") {
        if (!eq(abs(a.x - b.x), abs(a.y - b.y))) throw std::invalid_argument("Square must have equal sides");
    }
    Square(const Square& s) = default;
};


class Polyline : public Shape, public Named {
protected:
    Polyline(const char* name) : Named(name) {}

public:
    Polyline() : Polyline("Polyline") {}
    Polyline(const Polyline& p) = default;

    virtual Polyline& addPoint(const Point& p) {
        points.addLast(p);
        return *this;
    }

    const std::string print() const override {
        if (points.getSize() < 2) throw VertexUnderflow();

        std::stringstream s;
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
    Polygon() : Polyline("Polygon"), complete(false) {}
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

    const std::string print() const override {
        if (!complete) throw Incomplete();
        std::string s = Polyline::print();
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
