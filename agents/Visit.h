#ifndef OPTIMIZATION_MODELS_VISIT_H
#define OPTIMIZATION_MODELS_VISIT_H


#include "Location.h"

class Visit : public Location {
protected:
    double x;
    double y;
    unsigned int slot;
    unsigned int visitors;
    Visit(unsigned int slot, double x, double y, unsigned int visitors);

public:
    static Visit read();
    unsigned int getSlot() const;
    unsigned int getVisitors();
    Type getType() const override { return Type::VISIT; }
};


#endif //OPTIMIZATION_MODELS_VISIT_H
