#include <iostream>
#include "Agent.h"

Agent::Agent(double x, double y) {
    this->x = x;
    this->y = y;
}

Agent Agent::read() {
    double x, y;
    std::cin >> x >> y;
    return Agent(x, y);
}
