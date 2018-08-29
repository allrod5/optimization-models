#ifndef OPTIMIZATION_MODELS_AGENT_H
#define OPTIMIZATION_MODELS_AGENT_H


#include "Home.h"

class Agent {

private:
    Home home;

    explicit Agent(Home home);

public:
    static Agent read();
    Home &getHome();
};


#endif //OPTIMIZATION_MODELS_AGENT_H
