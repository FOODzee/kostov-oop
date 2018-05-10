#pragma once

#include "Shapes.h"

enum ShapeKind {
    point,
    circle,
    rect,
    square,
    polyline,
    polygon,

    First = point,
    Last = polygon
};

int RandomInt();

namespace ShapeFactory {
    Shape* GenAnyShape();
    Shape* GenShape(ShapeKind kind);

    Point    GenPoint();
    Circle   GenCircle();
    Rect     GenRect();
    Square   GenSquare();
    Polyline GenPolyline();
    Polygon  GenPolygon();
}
