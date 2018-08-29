#ifndef OPTIMIZATION_MODELS_LOCATION_H
#define OPTIMIZATION_MODELS_LOCATION_H

class Location {
protected:
    double x{};
    double y{};

public:

    enum Type {
        VISIT,
        HOME
    };

    double getX() const {
        return x;
    }

    double getY() const {
        return y;
    }

    virtual Type getType() const {
        return Type::VISIT;
    }
};


#endif //OPTIMIZATION_MODELS_LOCATION_H
