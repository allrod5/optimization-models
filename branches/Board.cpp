#include <utility>
#include "Board.h"

Board::Board(unsigned int width, unsigned long height, std::vector<std::string> board)
        : width(width), height(height), board(std::move(board)) {
}

Board Board::read() {
    unsigned int width;
    unsigned int height;
    std::vector<std::string> board;

    std::cin >> width >> height;
    board.resize(height);
    for (int i = 0; i < height; ++i) {
        std::cin >> board[i];
    }

    return Board(width, height, board);
}

char & Board::at(unsigned int x, unsigned int y) {
    return board[x][y];
}

bool Board::contains(unsigned int x, unsigned int y) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

unsigned int Board::getWidth() {
    return width;
}

unsigned int Board::getHeight() {
    return height;
}
