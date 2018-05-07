#include <ctime>
#include <random>
#include <functional>
#include <stdexcept>
#include "ShapeFactory.h"
#include "Shapes.h"

unsigned int Shape::count = 0;

std::mt19937 rng(time(0));

std::uniform_int_distribution<int> shapes(ShapeKind::First, ShapeKind::Last);
auto randomShapeKind = std::bind(shapes, rng);

std::normal_distribution<double> doubles(3, 10);
auto randomDouble = std::bind(doubles, rng);

std::poisson_distribution<int> ints(2);
auto randomInt = std::bind(ints, rng);

int RandomInt() { return randomInt(); }

namespace ShapeFactory {

    Shape* GenAnyShape() {
        return GenShape(static_cast<ShapeKind>(randomShapeKind()));
    }

    Shape* GenShape(ShapeKind kind) {
        switch (kind) {
            case point:    return new Point   (GenPoint());
            case circle:   return new Circle  (GenCircle());
            case rect:     return new Rect    (GenRect());
            case square:   return new Square  (GenSquare());
            case polyline: return new Polyline(GenPolyline());
            case polygon:  return new Polygon (GenPolygon());
            default:       throw std::invalid_argument("unknown shape kind requested");
        }
    }

    Point GenPoint() {
        return Point(randomDouble(), randomDouble());
    }

    Circle GenCircle() {
        double r;
        do {
            r = randomDouble();
        } while (r <= 0);

        return Circle(GenPoint(), r);
    }

    Rect GenRect() {
        while (true) {
            try {
                return Rect(GenPoint(), GenPoint());
            } catch (std::invalid_argument err) {
                continue; // retry
            }
        }
    }

    Square GenSquare() {
        Point a = GenPoint();
        double side = randomDouble();
        Point b(a.x + side, a.y + side);
        return Square(a, b);
    }

    Polyline GenPolyline() {
        Polyline l;
        int n = randomInt() + 2;
        while (n-- > 0) {
            l.addPoint(GenPoint());
        }
        return l;
    }

    Polygon GenPolygon() {
        Polygon p;
        Point o = GenPoint();
        p.addPoint(o);

        int n = randomInt() + 2;
        while (n-- > 0) {
            Point x = o;
            do {
                x = GenPoint();
            } while (x == o);
            p.addPoint(x);
        }

        p.addPoint(o); // close up border

        return p;
    }

}