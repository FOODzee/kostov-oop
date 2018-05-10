#pragma once

#include <string>
#include <limits>

class Printable {
public:
    virtual const std::string print() const = 0;
};

class Named : public Printable {
public:
    Named(const char* name) : Named(std::string(name)) {}
    Named(const std::string& name) : name(name) {}
    Named(const Named& n) : name(n.name) {}

    const std::string print() const override {
        return name;
    }

    const std::string name;
};
