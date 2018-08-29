#include <cmath>
#include "Board.h"
#include "../easyscip/easyscip.h"
#include "branches.h"

std::vector<Group>
getGroups(Board &board);

std::vector<std::vector<std::vector<easyscip::Variable>>>
initializeSolverVariables(Board &board, std::vector<Group> &groups, easyscip::MIPSolver &solver);

void
constraintGroupsBranches(
        easyscip::MIPSolver &solver, Board &board, std::vector<Group> &groups,
        std::vector<std::vector<std::vector<easyscip::Variable>>> &variables);

void
constraintBranchesPerTile(
        easyscip::MIPSolver &solver, Board &board, std::vector<Group> &groups,
        std::vector<std::vector<std::vector<easyscip::Variable>>> &variables);

bool
withinGroupRange(Group &group, unsigned int i, unsigned int j);

int
fromToIncrement(unsigned int from, unsigned int to);

void
printSolvedBoard(
        easyscip::Solution& solution, Board& board, std::vector<Group>& groups,
        std::vector<std::vector<std::vector<easyscip::Variable>>>& variables);

int
distance(unsigned int i, unsigned int j, Group &group);

void
branches() {
    Board board = Board::read();
    auto groups = getGroups(board);
    auto solver = easyscip::MIPSolver();
    auto variables = initializeSolverVariables(board, groups, solver);
    constraintBranchesPerTile(solver, board, groups, variables);
    constraintGroupsBranches(solver, board, groups, variables);
    auto solution = solver.solve();
    printSolvedBoard(solution, board, groups, variables);

}

void
constraintBranchesPerTile(
        easyscip::MIPSolver &solver, Board &board, std::vector<Group> &groups,
        std::vector<std::vector<std::vector<easyscip::Variable>>> &variables) {
    for (unsigned int i = 0; i < board.getHeight(); i++) {
        for (unsigned int j = 0; j < board.getWidth(); j++) {
            auto constraint = solver.constraint();
            for (unsigned int group = 0; group < groups.size(); group++) {
                constraint.add_variable(variables[i][j][group], 1);
            }
            auto value = board.at(i, j) == '.' ? 1 : 0;
            constraint.commit(value, value);
        }
    }
}

void
constraintGroupsBranches(
        easyscip::MIPSolver &solver, Board &board, std::vector<Group> &groups,
        std::vector<std::vector<std::vector<easyscip::Variable>>> &variables) {
    auto outOfGroupsRangeConstraint = solver.constraint();
    for (unsigned int g = 0; g < groups.size(); g++) {
        auto group = groups[g];
        auto groupBranchesCountConstraint = solver.constraint();
        for (unsigned int i = 0; i < board.getHeight(); i++) {
            for (unsigned int j = 0; j < board.getWidth(); j++) {
                if (i == group.x && j == group.y) {
                    continue;
                }

                if (!withinGroupRange(group, i, j)) {
                    outOfGroupsRangeConstraint.add_variable(variables[i][j][g], 1);
                    continue;
                }

                groupBranchesCountConstraint.add_variable(variables[i][j][g], 1);

                auto constraint = solver.constraint();
                constraint.add_variable(variables[i][j][g], 1 - distance(i, j, group));
                int dx = fromToIncrement(i, group.x);
                int dy = fromToIncrement(j, group.y);
                int x = i + dx;
                int y = j + dy;
                while (x != group.x || y != group.y) {
                    constraint.add_variable(variables[x][y][g], 1);
                    x += dx;
                    y += dy;
                }
                constraint.commit(0, distance(i, j, group) - 1);
            }
        }
        groupBranchesCountConstraint.commit(group.branches, group.branches);
    }
    outOfGroupsRangeConstraint.commit(0, 0);
}

int
distance(unsigned int i, unsigned int j, Group &group) {
    return std::abs((int)(group.x - i + group.y - j));
}

bool
withinGroupRange(Group &group, unsigned int i, unsigned int j) {
    return (i == group.x && group.branches >= std::abs((int) (group.x - i)))
           || (j == group.y && group.branches >= std::abs((int) (group.y - j)));
}

int
fromToIncrement(unsigned int from, unsigned int to) {
    int difference = to - from;
    return difference != 0 ? difference / std::abs(difference) : 0;
}

std::vector<Group>
getGroups(Board &board) {
    std::vector<Group> groups;
    for (unsigned int i = 0; i < board.getHeight(); ++i) {
        for (unsigned int j = 0; j < board.getWidth(); ++j) {
            char value = board.at(i, j);
            if (value == '.') {
                continue;
            }
            groups.push_back(Group{
                    .branches = (unsigned int) atoi(&value),
                    .x = i,
                    .y = j
            });
        }
    }
    return groups;
}

std::vector<std::vector<std::vector<easyscip::Variable>>>
initializeSolverVariables(Board &board, std::vector<Group> &groups, easyscip::MIPSolver &solver) {
    std::vector<std::vector<std::vector<easyscip::Variable>>> variables(board.getHeight());
    for (int i = 0; i < board.getHeight(); ++i) {
        variables[i] = std::vector<std::vector<easyscip::Variable>>(board.getWidth());
        for (int j = 0; j < board.getWidth(); ++j) {
            for (unsigned int group = 0; group < groups.size(); group++) {
                variables[i][j].push_back(solver.binary_variable(1));
            }
        }
    }
    return variables;
}

void
printSolvedBoard(
        easyscip::Solution& solution, Board& board, std::vector<Group>& groups,
        std::vector<std::vector<std::vector<easyscip::Variable>>>& variables) {
    for (int k = 0; k < groups.size(); k++) {
        for (int j = 0; j < board.getHeight(); j++) {
            for (int i = 0; i < board.getWidth(); i++) {
                if (solution.value(variables[j][i][k]) > 0.5) {
                    std::cout << '*';
                } else {
                    std::cout << board.at(j, i);
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
