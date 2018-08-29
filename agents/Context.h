#ifndef OPTIMIZATION_MODELS_CONTEXT_H
#define OPTIMIZATION_MODELS_CONTEXT_H


#include <vector>
#include "Agent.h"
#include "Visit.h"

class Context {
private:
    std::vector<Agent> agents;
    std::vector<Visit> visits;
    unsigned int slots;
    Context(std::vector<Agent> agents, std::vector<Visit> visits);

public:

    static Context read();
    std::vector<Agent> & getAgents();
    std::vector<Visit> & getVisits();
    unsigned int getSlots();
};


#endif //OPTIMIZATION_MODELS_CONTEXT_H
