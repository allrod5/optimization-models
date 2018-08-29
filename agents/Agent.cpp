#include <utility>

#include <iostream>
#include "Agent.h"

Agent::Agent(Home home) : home(std::move(home)) {}

Agent Agent::read() {
    auto home = Home::read();
    return Agent(home);
}

Home &Agent::getHome() {
    return home;
}
