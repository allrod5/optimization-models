#include <iostream>
#include "Visit.h"

Visit::Visit(unsigned int slot, double x, double y, unsigned int visitors)
        : slot(slot), x(x), y(y), visitors(visitors) {}

unsigned int Visit::getSlot() {
    return slot;
}

unsigned int Visit::getVisitors() {
    return visitors;
}

Visit Visit::read() {
    unsigned int slot, visitors;
    double x, y;
    std::cin >> slot >> x >> y >> visitors;
    return Visit(slot, x, y, visitors);
}
