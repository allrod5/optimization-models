#ifndef OPTIMIZATION_MODELS_VISIT_H
#define OPTIMIZATION_MODELS_VISIT_H


class Visit {

private:
    unsigned int slot;
    double x;
    double y;
    unsigned int visitors;
    Visit(unsigned int slot, double x, double y, unsigned int visitors);

public:
    static Visit read();
    unsigned int getSlot();
    unsigned int getVisitors();
};


#endif //OPTIMIZATION_MODELS_VISIT_H
