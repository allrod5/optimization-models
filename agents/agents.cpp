#include "agents.h"
#include "../easyscip/easyscip.h"
#include "Context.h"

std::vector<std::vector<std::vector<easyscip::Variable>>>
initializeSolverVariables(Context &context, easyscip::MIPSolver &solver);

void
constraintMaximumCanceledVisits(Context &context, std::vector<std::vector<easyscip::Variable>> &variables,
                                easyscip::MIPSolver &solver);

void
constraintMaximumAgentsPerVisit(std::vector<std::vector<easyscip::Variable>> &variables, easyscip::MIPSolver &solver);

void
agents() {
    Context context = Context::read();
    auto solver = easyscip::MIPSolver();
    auto variables = initializeSolverVariables(context, solver);
    constraintMaximumAgentsPerVisit(variables, solver);
    constraintMaximumCanceledVisits(context, variables, solver);
    auto solution = solver.solve();
    printSolvedBoard(solution, board, groups, variables);

}

void
constraintMaximumAgentsPerVisit(std::vector<std::vector<easyscip::Variable>> &variables, easyscip::MIPSolver &solver) {
    for (auto visit : variables) {
        auto constraint = solver.constraint();
        for (auto agent: visit) {
            constraint.add_variable(agent, 1);
        }
        constraint.commit(0, 1);
    }
}

std::vector<std::vector<std::vector<easyscip::Variable>>>
initializeSolverVariables(Context &context, easyscip::MIPSolver &solver) {
    std::vector<std::vector<easyscip::Variable>> variables(context.getVisits().size());
    for (int i = 0; i < context.getVisits().size(); ++i) {
        variables[i].push_back(solver.binary_variable(1));
    }
    return variables;
}

void
constraintMaximumCanceledVisits(
        Context &context, std::vector<std::vector<easyscip::Variable>> &variables,
        easyscip::MIPSolver &solver) {
    auto constraint = solver.constraint();
    int totalVisits = 0;
    for (int i = 0; i < context.getVisits().size(); ++i) {
        int effectiveVisits = context.getVisits()[i].getVisitors();
        totalVisits += effectiveVisits;
        for (int j = 0; j < context.getAgents().size(); ++j) {
            constraint.add_variable(variables[i][j], effectiveVisits);
        }
    }
    constraint.commit(totalVisits*0.95, totalVisits);
}