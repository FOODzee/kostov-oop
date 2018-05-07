#include <iostream>
#include "Container.h"
#include "Shapes.h"
#include "ShapeFactory.h"

int main() {
    auto c = Container<Shape*>( [](Shape* s) { delete s; } );

    int n = RandomInt() + 20;
    while(n-- > 0) {
        c.addLast(ShapeFactory::GenAnyShape());
    }

    std::cout << Shape::GetCount() << std::endl;

    c.forEach(
            [] (Shape* s) {
                std::cout << *s << std::endl;
            }
    );

    c.clear();

    std::cout << Shape::GetCount() << std::endl;

    return 0;
}