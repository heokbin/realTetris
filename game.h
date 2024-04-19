#ifndef GAME_H
#define GAME_H

#include "tetromino.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60

class Game {
private:
  int currTetroX;
  int currTetroY;
  Tetromino currTetro;
  Tetromino nextTetro;
  Tetromino holdTetro;
  bool createTetro;
  bool swapTetro;
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];

  // board_가 true인 곳에 고정된 블록을 그린다.
  void printboard_(bool board_[BOARD_WIDTH][BOARD_HEIGHT]);
  Tetromino spawnNewTetro();

  Tetromino spawnNullTetro();

  // 키 입력
  void handleInput();

  void hardDrop();

  // 키 입력 Down
  void softDrop();

  void printNextTetro();

  void printHoldTetro();

  void printShadow();

  void checkTetro();

  // 한 줄 됐는지 체크
  void checkLine();

  void boardMinus(int y);

  // 움직이는 테트로 출력
  void printTetro();

public:
  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();
};
#endif