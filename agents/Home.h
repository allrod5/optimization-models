//
// Created by rodrigo on 29/08/18.
//

#ifndef OPTIMIZATION_MODELS_HOME_H
#define OPTIMIZATION_MODELS_HOME_H


#include <cmath>
#include "Location.h"

class Home : public Location {
protected:
    double x;
    double y;
    explicit Home(double x, double y);

public:
    static Home read();
    Type getType() const override { return Type::HOME; }
};


#endif //OPTIMIZATION_MODELS_HOME_H
