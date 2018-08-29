#include <utility>

#include <iostream>
#include "Context.h"

Context Context::read() {
    unsigned int numberOfAgents, numberOfVisits;
    std::vector<Agent> agents;
    std::vector<Visit> visits;

    std::cin >> numberOfAgents;
    agents.reserve(numberOfAgents);
    for (int i = 0; i < numberOfAgents; ++i) {
        agents.push_back(Agent::read());
    }

    std::cin >> numberOfVisits;
    visits.reserve(numberOfVisits);
    for (int i = 0; i < numberOfVisits; ++i) {
        visits.push_back(Visit::read());
    }

    return Context(agents, visits);
}

std::vector<Agent> & Context::getAgents() {
    return agents;
}

std::vector<Visit> & Context::getVisits() {
    return visits;
}

Context::Context(std::vector<Agent> agents, std::vector<Visit> visits) : agents(std::move(agents)),
                                                                         visits(std::move(visits)) {}
