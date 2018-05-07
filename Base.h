#ifndef KOSTOV_OOP_BASE_H
#define KOSTOV_OOP_BASE_H

#include <string>
#include <limits>

using namespace std;

class Printable {
public:
    virtual const string print() const = 0;
};

class Named : public Printable {
public:
    Named(const string& name) : name(name) {}
    Named(const Named& n) : name(n.name) {}

    const string print() const override {
        return name;
    }

    const string name;
};

#endif //KOSTOV_OOP_BASE_H
