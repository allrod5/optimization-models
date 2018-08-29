#ifndef OPTIMIZATION_MODELS_BOARD_H
#define OPTIMIZATION_MODELS_BOARD_H

#include <iostream>
#include <vector>

class Board {
private:
    unsigned int width;
    unsigned long height;
    std::vector<std::string> board;
    Board(unsigned int width, unsigned long height, std::vector<std::string> board);

public:
    static Board read();

    char & at(unsigned int x, unsigned int y);
    bool contains(unsigned int x, unsigned int y);
    unsigned int getWidth();
    unsigned int getHeight();
};


#endif //OPTIMIZATION_MODELS_BOARD_H
