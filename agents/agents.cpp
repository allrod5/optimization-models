#include <iostream>
#include "agents.h"
#include "../easyscip/easyscip.h"
#include "Context.h"

std::vector<std::vector<std::vector<easyscip::Variable>>>
initializeSolverAssignmentVariables(Context &context, easyscip::MIPSolver &solver);

void
constraintMaximumCanceledVisits(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &assignmentVariables,
        easyscip::MIPSolver &solver);

void
constraintMaximumAgentsPerVisit(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &assignmentVariables,
        easyscip::MIPSolver &solver);

void
constraintSlotsPerVisit(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &assignmentVariables,
        easyscip::MIPSolver &solver);

void
constraintSlotsPerAgent(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &assignmentVariables,
        easyscip::MIPSolver &solver);

void
printSolution(
        easyscip::Solution &solution, Context &context,
        std::vector<std::vector<std::vector<easyscip::Variable>>> &assignmentVariables,
        std::vector<std::vector<std::vector<easyscip::Variable>>> &costVariables);

std::vector<std::vector<std::vector<easyscip::Variable>>>
initializeSolverCostVariables(Context &context, easyscip::MIPSolver &solver);

double
cost(Location &origin, Location &destination);

void
constraintAssignmentVariablesWithCostVariables(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &vector,
        std::vector<std::vector<std::vector<easyscip::Variable>>> &variables, easyscip::MIPSolver &solver);

void
constraintLeaveAndArriveHomeCost(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &costVariables,
        easyscip::MIPSolver &solver);
void
constraintMaximumCostingPerVisit(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &costVariables,
        easyscip::MIPSolver &solver);

void
agents() {
    Context context = Context::read();
    auto solver = easyscip::MIPSolver();
    auto assignmentVariables = initializeSolverAssignmentVariables(context, solver);
    auto costVariables = initializeSolverCostVariables(context, solver);
//    constraintSlotsPerVisit(context, assignmentVariables, solver);
    constraintAssignmentVariablesWithCostVariables(context, assignmentVariables, costVariables, solver);
    constraintLeaveAndArriveHomeCost(context, costVariables, solver);
    constraintMaximumCostingPerVisit(context, costVariables, solver);
    constraintMaximumAgentsPerVisit(context, assignmentVariables, solver);
    constraintMaximumCanceledVisits(context, assignmentVariables, solver);
    constraintSlotsPerAgent(context, assignmentVariables, solver);
    solver.set_time_limit(5000*60);
    auto solution = solver.solve();
    printSolution(solution, context, assignmentVariables, costVariables);

}

void
constraintLeaveAndArriveHomeCost(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &costVariables,
        easyscip::MIPSolver &solver) {
    for (int a = 0; a < context.getAgents().size(); ++a) {
        auto leaveHomeConstraint = solver.constraint();
        auto arriveHomeConstraint = solver.constraint();
        for (int i = 0; i <= context.getVisits().size(); ++i) {
            leaveHomeConstraint.add_variable(costVariables[a][i][context.getVisits().size()], 1);
            arriveHomeConstraint.add_variable(costVariables[a][context.getVisits().size()][i], 1);
        }
        leaveHomeConstraint.commit(1, 1);
        arriveHomeConstraint.commit(1, 1);
    }
}

void
constraintMaximumCostingPerVisit(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &costVariables,
        easyscip::MIPSolver &solver) {
    for (int a = 0; a < context.getAgents().size(); ++a) {
        for (int i = 0; i <= context.getVisits().size(); ++i) {
            auto originConstraint = solver.constraint();
            auto destinationConstraint = solver.constraint();
            for (int j = 0; j <= context.getVisits().size(); ++j) {
                originConstraint.add_variable(costVariables[a][i][j], 1);
                destinationConstraint.add_variable(costVariables[a][j][i], 1);
            }
            originConstraint.commit(0, 1);
            destinationConstraint.commit(0, 1);
        }
    }
}

void
constraintAssignmentVariablesWithCostVariables(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &assignmentVariables,
        std::vector<std::vector<std::vector<easyscip::Variable>>> &costVariables, easyscip::MIPSolver &solver) {
    for (int a = 0; a < context.getAgents().size(); ++a) {
        for (int i = 0; i <= context.getVisits().size(); ++i) {
            bool fromHouse = i == context.getVisits().size();
            for (int j = 0; j <= context.getVisits().size(); ++j) {
                if (i == j) {
                    continue;
                }
                bool toHouse = j == context.getVisits().size();
                auto constraint = solver.constraint();
                int fromSlot=-1, toSlot=-1;
                int multiplier=0;
                if (not fromHouse) {
                    multiplier--;
                    fromSlot = context.getVisits()[i].getSlot();
                    constraint.add_variable(assignmentVariables[a][i][fromSlot], 1);
                }
                if (not toHouse) {
                    multiplier--;
                    toSlot = context.getVisits()[j].getSlot();
                    constraint.add_variable(assignmentVariables[a][j][toSlot], 1);
                }
                if (not fromHouse and not toHouse and fromSlot >= toSlot) {
                    continue;
                }
                constraint.add_variable(costVariables[a][i][j], multiplier);
                constraint.commit(0, 1);
            }
        }
    }
}

std::vector<std::vector<std::vector<easyscip::Variable>>>
initializeSolverAssignmentVariables(Context &context, easyscip::MIPSolver &solver) {
    std::vector<std::vector<std::vector<easyscip::Variable>>> assignmentVariables(context.getAgents().size());
    for (int i = 0; i < context.getAgents().size(); ++i) {
        assignmentVariables[i] = std::vector<std::vector<easyscip::Variable>>(context.getVisits().size());
        for (int j = 0; j < context.getVisits().size(); ++j) {
            for (int k = 0; k < context.getSlots(); ++k) {
                auto variable = context.getVisits()[j].getSlot() == k
                                ? solver.binary_variable(1) : solver.null_variable();
                assignmentVariables[i][j].push_back(variable);
            }
        }
    }
    return assignmentVariables;
}

std::vector<std::vector<std::vector<easyscip::Variable>>>
initializeSolverCostVariables(Context &context, easyscip::MIPSolver &solver) {
    std::vector<std::vector<std::vector<easyscip::Variable>>> costVariables(context.getAgents().size());
    for (int a = 0; a < context.getAgents().size(); ++a) {
        costVariables[a] = std::vector<std::vector<easyscip::Variable>>(context.getVisits().size()+1);
        for (int i = 0; i <= context.getVisits().size(); ++i) {
            bool originIsVisit = true;
            if (i == context.getVisits().size()) {
                originIsVisit = false;
            }
            for (int j = 0; j <= context.getVisits().size(); ++j) {
                bool destinationIsVisit = true;
                if (j == context.getVisits().size()) {
                    destinationIsVisit = false;
                }
                if (originIsVisit and destinationIsVisit) {
                    costVariables[a][i].push_back(context.getVisits()[i].getSlot() < context.getVisits()[j].getSlot()
                                                  ? solver.binary_variable(cost(context.getVisits()[i], context.getVisits()[j])) : solver.null_variable());
                } else if (not originIsVisit and not destinationIsVisit) {
                    costVariables[a][i].push_back(
                            solver.binary_variable(0));
                } else if (not originIsVisit) {
                    costVariables[a][i].push_back(
                            solver.binary_variable(cost(context.getAgents()[a].getHome(), context.getVisits()[j])));
                } else {
                    costVariables[a][i].push_back(
                            solver.binary_variable(cost(context.getVisits()[i], context.getAgents()[a].getHome())));
                }
            }
        }
    }
    return costVariables;
}

void
constraintSlotsPerVisit(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &assignmentVariables,
        easyscip::MIPSolver &solver) {
    for (auto agent : assignmentVariables) {
        for (auto visit : agent) {
            auto constraint = solver.constraint();
            for (auto slot : visit) {
                constraint.add_variable(slot, 1);
            }
            constraint.commit(0, 1);
        }
    }
}

void
constraintMaximumAgentsPerVisit(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &assignmentVariables,
        easyscip::MIPSolver &solver) {
    for (int j = 0; j < context.getVisits().size(); ++j) {
        int slot = context.getVisits()[j].getSlot();
        auto constraint = solver.constraint();
        for (int i = 0; i < context.getAgents().size(); ++i) {
            constraint.add_variable(assignmentVariables[i][j][slot], 1);
        }
        constraint.commit(0, 1);
    }
}


void
constraintMaximumCanceledVisits(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &assignmentVariables,
        easyscip::MIPSolver &solver) {
    auto constraint = solver.constraint();
    int totalVisits = 0;
    for (int j = 0; j < context.getVisits().size(); ++j) {
        int slot = context.getVisits()[j].getSlot();
        int effectiveVisits = context.getVisits()[j].getVisitors();
        totalVisits += effectiveVisits;
        for (int i = 0; i < context.getAgents().size(); ++i) {
            constraint.add_variable(assignmentVariables[i][j][slot], effectiveVisits);
        }
    }
    constraint.commit(0.95 * totalVisits, totalVisits);
}

void
constraintSlotsPerAgent(
        Context &context, std::vector<std::vector<std::vector<easyscip::Variable>>> &assignmentVariables,
        easyscip::MIPSolver &solver) {
    for (auto agent : assignmentVariables) {
        for (int k = 0; k < context.getSlots(); ++k) {
            auto constraint = solver.constraint();
            for (int j = 0; j < context.getVisits().size(); ++j) {
                constraint.add_variable(agent[j][k], 1);
            }
            constraint.commit(0, 1);
        }
    }
}

double
cost(Location &origin, Location &destination) {
    return -std::hypot(destination.getX() - origin.getX(), destination.getY() - origin.getY());
}

void
printSolution(
        easyscip::Solution &solution, Context &context,
        std::vector<std::vector<std::vector<easyscip::Variable>>> &assignmentVariables,
        std::vector<std::vector<std::vector<easyscip::Variable>>> &costVariables) {
    for (int j = 0; j < context.getVisits().size(); ++j) {
        bool assigned = false;
        int slot = context.getVisits()[j].getSlot();
        for (int i = 0; i < context.getAgents().size(); ++i) {
            if (solution.value(assignmentVariables[i][j][slot]) <= 0.5) continue;
            std::cout << "\nVisit " << j << " [slot=" << slot << "] was assigned to agent " << i;
            assigned = true;
        }
        if (not assigned) {
            std::cout << "\nVisit " << j << " [slot=" << slot << "] was cancelled";
        }
    }

    for (int a = 0; a < context.getAgents().size(); a ++) {
        std::cout << "\n\nAgent " << a << ": ";
        for (int i = 0; i <= context.getVisits().size(); i++) {
            for (int j = 0; j <= context.getVisits().size(); j++) {
                if (solution.value(costVariables[a][i][j]) <= 0.5) continue;
                if (i == context.getVisits().size() && j == context.getVisits().size()) {
                    std::cout << "Remained at Home ::: ";
                } else if (i == context.getVisits().size()) {
                    std::cout << "From Home went to Visit " << j << " [slot=" << context.getVisits()[j].getSlot() << "] ::: ";
                } else if (j == context.getVisits().size()) {
                    std::cout << "From Visit " << i << " [slot=" << context.getVisits()[i].getSlot() << "] to Home ::: ";
                } else {
                    std::cout << "From Visit " << i << " [slot=" << context.getVisits()[i].getSlot() << "] to Visit " << j << " [slot=" << context.getVisits()[j].getSlot() << "] :::";
                }
            }
        }
    }
}
