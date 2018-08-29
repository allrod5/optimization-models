#ifndef OPTIMIZATION_MODELS_AGENT_H
#define OPTIMIZATION_MODELS_AGENT_H


class Agent {

private:
    double x;
    double y;
    Agent(double x, double y);

public:
    static Agent read();
};


#endif //OPTIMIZATION_MODELS_AGENT_H
