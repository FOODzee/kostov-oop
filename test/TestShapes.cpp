#include "catch.hpp"
#include "../Shapes.h"

unsigned int Shape::count = 0;

TEST_CASE( "Points equality" ) {
    Point p1(1, 1);
    Point p2(1, 1);
    Point p3(0, 0);

    REQUIRE(p1 == p2);
    REQUIRE(p1 != p3);
}

TEST_CASE( "Shapes counting" ) {
    REQUIRE( Shape::GetCount() == 0 );
    {
        Point p(1, 1);
        REQUIRE( Shape::GetCount() == 1 );
        {
            Circle c(p, 5);
            REQUIRE( Shape::GetCount() == 3 ); // circle creates its own copy of Point
        } // c got destructed
        REQUIRE( Shape::GetCount() == 1 );
    } // p got destructed
    REQUIRE( Shape::GetCount() == 0 );
}

TEST_CASE( "Square & Rect area" ) {
    REQUIRE( Square(Point(0, 0), Point(1, 1)).area() == 1 );
    REQUIRE( Rect(Point(0, 0), Point(1, 2)).area() == 2 );
}

TEST_CASE( "Polyline length" ) {
    Polyline l;                    //  1 +-+
    l.addPoint(Point(0, 0));       //    ! +
    l.addPoint(Point(0, 1));       //  0 +
    l.addPoint(Point(0.5, 1));     //    0 .5
    l.addPoint(Point(0.5, 0.5));   //
    REQUIRE( l.length() == 2 );
}

TEST_CASE( "Polygon perimeter" ) {
    Polygon p;                     //  1 +-+
    p.addPoint(Point(0, 0));       //    ! +
    p.addPoint(Point(0, 1));       //  0 +'
    p.addPoint(Point(0.5, 1));     //    0 .5
    p.addPoint(Point(0.5, 0.5));
    REQUIRE_THROWS_AS( p.perimeter() == 2, Polygon::Incomplete );
    
    p.addPoint(Point(0, 0));
    REQUIRE( p.perimeter() == 2. + sqrt(2)/2. );

    REQUIRE_THROWS_AS( p.addPoint(Point(4, 2)), Polygon::Complete );
}