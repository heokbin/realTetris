#include "game.h"
#include "console/console.h"
#include "tetromino.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <random>
using namespace std;

void printNext();
void printHold();
void printLines();
void printTime();
void win();

int lines = LINES;

int softNum = 0;

Game::Game(): currTetro(spawnNewTetro()), nextTetro(spawnNewTetro()), holdTetro(spawnNullTetro()), board_{false, } {
    currTetroX = BOARD_WIDTH / 2;
    currTetroY = 1;
    createTetro = false;
    swapTetro = true;
}

void Game::update() {
    softNum++;
    if (softNum == 60) {
        checkTetro();
        softDrop();
        softNum = 0;
    }
    else {
        handleInput();
    }
    checkLine();

    if (createTetro) {
        currTetro = nextTetro;
        nextTetro = spawnNewTetro();
        currTetroX = BOARD_WIDTH / 2;
        currTetroY = 1;
        createTetro = false;
        swapTetro = true;
    }
}

void Game::draw() {
    printTime();
    printLines();
    if (lines <= 0) {
        win();
    }

    console::drawBox(0, 0, BOARD_WIDTH+1, BOARD_HEIGHT+1);
    console::drawBox(13, 0, 18, 5);
    console::drawBox(19, 0, 24, 5);

    printNext();
    printNextTetro();
    printHold();
    printHoldTetro();
    printboard_(board_);
    
    if (lines > 0 && !shouldExit()) {
        printShadow();
        printTetro();
    }
}

int minute = 0;
int second = 0;
int ms = 0;
string tetrisTime = "";
void printTime() {
        ms += 1000 / DROP_DELAY;

    if (ms >= 1000) {
        second++;
        ms -= 1000;
    }

    if (second == 60) {
        minute++;
        second = 0;
    }
    tetrisTime = to_string(minute/10) + to_string(minute%10) + ":" + to_string(second/10) + to_string(second%10) + ":";
    
    string printms;
    if (ms < 10) {
        printms = "00";
    }
    else if (ms < 100) {
        printms = "0" + to_string(ms / 10);
    }
    else {
        printms = to_string(ms / 10);
    }
    tetrisTime += printms;
    console::draw(2, BOARD_HEIGHT + 3, tetrisTime);
}

void win() {
    console::draw(3,10,"You Win");
    console::draw(2,11,tetrisTime);
}

void printLines() {
    int x = 0;
    if (lines <= 0) {
        console::draw(x, BOARD_HEIGHT + 2, "0 lines left");
        return;
    }
    int tenLines = lines / 10;
    int oneLines = lines % 10;
    if (tenLines != 0) {
        console::draw(x++, BOARD_HEIGHT + 2, to_string(tenLines));
    }
    console::draw(x++, BOARD_HEIGHT + 2, to_string(oneLines));
    console::draw(x++, BOARD_HEIGHT + 2, " lines left");
}

void Game::printNextTetro() {
    int x = 15;
    int y = 2;
    if (nextTetro.name() == "I") {
        x--;
        nextTetro.drawAt(BLOCK_STRING, x ,y);
    }
    else {
        nextTetro.drawAt(BLOCK_STRING, x ,y);
    }
}

void Game::printHoldTetro() {
    int x = 21;
    int y = 2;
    if (holdTetro.name() == "I") {
        x--;
        holdTetro.drawAt(BLOCK_STRING, x ,y);
    }
    else {
        holdTetro.drawAt(BLOCK_STRING, x ,y);
    }
}

void printNext() {
    console::draw(14, 0, "Next");
}

void printHold() {
    console::draw(20, 0, "Hold");
}

void Game::printTetro() {
    currTetro.drawAt(BLOCK_STRING, currTetroX ,currTetroY);
}


void Game::printShadow() {
    int i = currTetroY;
    bool shadow = true;
    while(shadow) {
        for (int y = i; y < i + currTetro.size(); y++) {
            for (int x = currTetroX; x < currTetroX + currTetro.size(); x++) {
                if (currTetro.check(x - currTetroX, y - i)) {
                    
                    if (y != BOARD_HEIGHT && board_[x-1][y]) {
                            shadow = false;
                    }
                    else if (y == BOARD_HEIGHT) {
                        shadow = false;
                        //num = 1;
                    }
                }
            }
        }
        i++;
    }
    i--;
    currTetro.drawAt(SHADOW_STRING, currTetroX, i);
}

void Game::checkTetro() {
    bool changeBoard = false;
    int arrX[4];
    int arrY[4];
    int i = 0;
    for (int y = currTetroY; y < currTetroY + currTetro.size(); y++) {
        for (int x = currTetroX; x < currTetroX + currTetro.size(); x++) {
            if (currTetro.check(x - currTetroX, y - currTetroY)) {
                arrX[i] = x;
                arrY[i] = y;
                i++;
                if (y == BOARD_HEIGHT) {
                    changeBoard = true;
                }
                else if (board_[x-1][y]) {
                    changeBoard = true;
                }
            }
        }
    }
    if (changeBoard) {
        createTetro = true;
        swapTetro = true;
        for (int num = 0; num < 4; num++) {
            board_[arrX[num] - 1][arrY[num] - 1] = true;
        }
    }
}

void Game::checkLine() {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        int num = 0;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board_[x][y]) {
                num++;
            }
        }
        if (num == BOARD_WIDTH) {
            boardMinus(y);
        }
    }
}

void Game::boardMinus(int minusY) {
    for (int y = minusY; y > 1; y--) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            board_[x][y] = board_[x][y-1];
        }
    }
    for (int x = 0; x < BOARD_WIDTH; x++) {
            board_[x][0] = false;
    }
    lines--;
}

void Game::printboard_(bool board_[BOARD_WIDTH][BOARD_HEIGHT]) {
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if (board_[x][y] == true) {
                console::draw(x + 1, y + 1, BLOCK_STRING);
            }
        }
    }
}

bool Game::shouldExit() {
    if (lines <= 0) {
        return true;
    }
    for (int x = 0; x < BOARD_WIDTH; x++) {
        if (board_[x][0] == true) {
            console::draw(2,10,"You Lost");
            return true;
        }
    }
    for (int y = currTetroY; y < currTetroY + currTetro.size(); y++) {
        for (int x = currTetroX; x < currTetroX + currTetro.size(); x++) {
            if (currTetro.check(x - currTetroX, y - currTetroY)) {
                if (board_[x-1][y-1]) {
                    console::draw(2,10,"You Lost");
                    return true;
                }
            }
        }
    }
    return false;
}

Tetromino Game::spawnNewTetro() {
    Tetromino newTetro = spawnNullTetro();
    int num = rand() % 7;
    
    switch (num) {
        case 0:
            newTetro = Tetromino::I;
            return newTetro;
        case 1:
            newTetro = Tetromino::T;
            return newTetro;
        case 2:
            newTetro = Tetromino::O;
            return newTetro;
        case 3:
            newTetro = Tetromino::S;
            return newTetro;
        case 4:
            newTetro = Tetromino::Z;
            return newTetro;
        case 5:
            newTetro = Tetromino::J;
            return newTetro;
        case 6:
            newTetro = Tetromino::L;
            return newTetro;
    }
}

Tetromino Game::spawnNullTetro() {
    Tetromino nullTetro("", 0, "");
    return nullTetro;
}

void Game::softDrop() {
    bool down = true;
    for (int y = currTetroY; y < currTetroY + currTetro.size(); y++) {
        for (int x = currTetroX; x < currTetroX + currTetro.size(); x++) {
            if (currTetro.check(x - currTetroX, y - currTetroY)) {
                if (y == BOARD_HEIGHT) {
                down = false;
                }
                else if (board_[x-1][y]){
                    down = false;
                }
            }
        }
    }
    
    if (down) {
        currTetroY++;
    }
}

void Game::handleInput() {
    if (console::key(console::K_ESC)) {
        exit(0);
    }
    else if (console::key(console::K_LEFT)) {
        bool left = true;
        for (int y = currTetroY; y < currTetroY + currTetro.size(); y++) {
            for (int x = currTetroX; x < currTetroX + currTetro.size(); x++) {
                if (currTetro.check(x - currTetroX, y - currTetroY)) {
                    if (x == 1) {
                        left = false;
                    }
                    else if(board_[x - 2][y-1]) {
                        left = false;
                    }
                }
            }
        }
        if (left) {
            currTetroX--;
        }
    }
    else if (console::key(console::K_RIGHT)) {
        bool right = true;
        for (int y = currTetroY; y < currTetroY + currTetro.size(); y++) {
            for (int x = currTetroX; x < currTetroX + currTetro.size(); x++) {
                if (currTetro.check(x - currTetroX, y - currTetroY)) {
                    if (x == BOARD_WIDTH) {
                        right = false;
                    }
                    else if(board_[x][y-1]) {
                        right = false;
                    }
                }
            }
        }
        if (right) {
            currTetroX++;
        }
    }
    else if (console::key(console::K_UP)) {
        hardDrop();
    }
    else if (console::key(console::K_DOWN)) {
        softDrop();
    }
    else if (console::key(console::K_Z)) {
        currTetro = currTetro.rotatedCW();
        bool rotated = true;
        for (int y = currTetroY; y < currTetroY + currTetro.size(); y++) {
            for (int x = currTetroX; x < currTetroX + currTetro.size(); x++) {
                if (currTetro.check(x - currTetroX, y - currTetroY)) {
                    if(x == BOARD_WIDTH + 1 || x == 0 || board_[x-1][y-1]) {
                        rotated = false;
                    }
                }
            }
        }
        if (!rotated) {
            currTetro = currTetro.rotatedCCW();
        }
    }
    else if (console::key(console::K_X)) {
        currTetro = currTetro.rotatedCCW();
        bool rotated = true;
        for (int y = currTetroY; y < currTetroY + currTetro.size(); y++) {
            for (int x = currTetroX; x < currTetroX + currTetro.size(); x++) {
                if (currTetro.check(x - currTetroX, y - currTetroY)) {
                    if(x == BOARD_WIDTH + 1 || x == 0 || board_[x-1][y-1]) {
                        rotated = false;
                    }
                }
            }
        }
        if (!rotated) {
            currTetro = currTetro.rotatedCW();
        }
    }
    else if (console::key(console::K_SPACE)) {
        static bool firstSwapTetro = true;
        if (swapTetro) {
            if (firstSwapTetro) {
                Tetromino* swapTetro = currTetro.original();
                currTetro = nextTetro;
                holdTetro = *swapTetro;
                nextTetro = spawnNewTetro();
                firstSwapTetro = false;
            }
            else {
                Tetromino* swapTetro = currTetro.original();
                Tetromino newHoldTetro = holdTetro;
                holdTetro = *swapTetro;
                currTetro = newHoldTetro;
            }
            currTetroX = BOARD_WIDTH / 2;
            currTetroY = 1;
        }
        swapTetro = false;
    }
}

void Game::hardDrop() {
    int i = currTetroY;
    bool shadow = true;
    int arrX[4];
    int arrY[4];
    while(shadow) {
        int num = 0;
        for (int y = i; y < i + currTetro.size(); y++) {
            for (int x = currTetroX; x < currTetroX + currTetro.size(); x++) {
                if (currTetro.check(x - currTetroX, y - i)) {
                    arrX[num] = x;
                    arrY[num] = y;
                    num++;
                    if (y != BOARD_HEIGHT) {
                        if (board_[x-1][y]) {
                            
                            shadow = false;

                        }
                    }
                    else {
                        shadow = false;
                    }
                }
            }
        }
        i++;
    }
    i--;
    createTetro = true;
    for (int num = 0; num < 4; num++) {
        board_[arrX[num] - 1][arrY[num] - 1] = true;
    }
}