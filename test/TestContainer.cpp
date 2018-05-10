#include <iostream>
#include "catch.hpp"
#include "../Container.h"

TEST_CASE( "empty" ) {
    auto c = Container<nullptr_t>();
    REQUIRE( c.isEmpty() );
}

TEST_CASE( "get/del on empty" ) {
    auto c = Container<nullptr_t>();
    REQUIRE_THROWS_AS( c.getFirst(), EmptyContainerException );
    c.delFirst();
}

TEST_CASE( "single" ) {
    const int v = 42;
    auto c = Container<int>();

    c.addFirst(v);

    REQUIRE( c.getSize() == 1 );
    REQUIRE( c.getFirst() == v );
}

TEST_CASE( "many in proper order" ) {
    const std::string array[] = {"several", "different", "strings"};
    auto c = Container<std::string>();

    for (const auto &s : array) {
        c.addLast(s);
    }

    for (const auto &s : array) {
        auto cs = c.getFirst();
        REQUIRE( cs == s );
        c.delFirst();
    }

    REQUIRE( c.isEmpty() );
}

TEST_CASE( "many can be cleared" ) {
    const std::string array[] = {"several", "different", "strings"};
    auto c = Container<std::string>();

    for (const auto &s : array) {
        c.addLast(s);
    }

    c.clear();

    REQUIRE( c.isEmpty() );
}

TEST_CASE( "forEach" ) {
    auto c = Container<int>();

    for (int i = -4; i < 42; i++) {
        c.addLast(i);
    }

    c.forEach(
            [](int i) { REQUIRE( i >= 0 ); },
            4 // skip 4 first (negative) numbers
    );
}

struct Box {
    Box(int i) : i(i) {}
    int i;
};

TEST_CASE( "forEach mutable" ) {
    auto c = Container<Box*>(
            // specify deleter to free Box on Container's destruction.
            [](Box* b) { delete b; }
    );

    // populate
    for (int i = -42; i < 42; i++) {
        c.addLast(new Box(i));
    }

    // mutate
    c.forEach(
            [](Box* b) { b->i += 42; }
    );

    // check
    c.forEach(
            [](Box* b) { REQUIRE( b->i >= 0 ); }
    );
}