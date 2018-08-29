#include <iostream>
#include "Home.h"


Home::Home(double x, double y) : x{x}, y{y} {}

Home Home::read() {
    double x, y;
    std::cin >> x >> y;
    return Home(x, y);
}
