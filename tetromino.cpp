#include "tetromino.h"
#include "console/console.h"
#include "game.h"
#include <iostream>
#include <string>
using namespace std;

Tetromino Tetromino::I("I", 4, "XXXXOOOOXXXXXXXX");

Tetromino Tetromino::T("T", 3, "XOXOOOXXX");

Tetromino Tetromino::O("O", 2, "OOOO");

Tetromino Tetromino::S("S", 3, "XOOOOXXXX");

Tetromino Tetromino::Z("Z", 3, "OOXXOOXXX");

Tetromino Tetromino::J("J", 3, "OXXOOOXXX");

Tetromino Tetromino::L("L", 3, "XXOOOOXXX");

Tetromino::Tetromino(std::string name, int size, std::string shape) {
    name_ = name;
    size_ = size;
    int i = 0;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            shape_[x][y] = (shape[i++] == 'O');
        }
    }
    original_ = this;
}
 // 시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
Tetromino Tetromino::rotatedCW() {
    Tetromino rotated(name_, size_);
    int i = 0;
    for (int x = 0; x < size_; x++) {
        for (int y = 0; y < size_; y++) {
            rotated.shape_[size_ - 1 - y][x] = check(x, y);
        }
    }
    rotated.original_ = original_;
    return rotated;
}
  // 반시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
Tetromino Tetromino::rotatedCCW() {
    Tetromino rotated(name_, size_);
    int i = 0;
    for (int x = 0; x < size_; x++) {
        for (int y = 0; y < size_; y++) {
            rotated.shape_[y][size_ - 1 - x] = check(x, y);
        }
    }
    rotated.original_ = original_;
    return rotated;
}

Tetromino::Tetromino(std::string name, int size) {
    name_ = name;
    size_ = size;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            shape_[x][y] = false;
        }
    }
}

void Tetromino::drawAt(std::string s, int x, int y) {
    for (int j = 0; j < size_; j++) {
        for (int i = 0; i < size_; i++) {
            if (check(i, j)) {
                console::draw(x + i, y + j, s);
            }
        }
    }
}