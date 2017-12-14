#include "tictactoe.h"


bool TicTacToe::insert(char type,
                       size_t x, size_t y) {
  if (_endGame) {
    return false;
  }
  if (_map[x][y] == ' ') {
    _map[x][y] = type;
    findWinArround(x, y);
    return true;
  } else {
    return false;
  }
}

void TicTacToe::findWinArround(size_t x,
                               size_t y) {
  const int derX[] = {-1, -1,  0,  1};
  const int derY[] = { 0, -1, -1, -1};

  bool winLine = false;
  int prevChangeX = -1, prevChangeY = -1;
  int curChangeX = -1, curChangeY = -1;
  int nLine = 0;

  for (int i = 0; i < 4; i++) {
    prevChangeX = x+derX[i]*(_winsN - 1);
    prevChangeY = y+derY[i]*(_winsN - 1);
    nLine = 1;
    for (int j = 1; j < _winsN+(_winsN - 1); j++) {
      curChangeX = x+derX[i]*(_winsN - 1 - j);
      curChangeY = y+derY[i]*(_winsN - 1 - j);

      if (inRange(curChangeX, curChangeY) &&
          inRange(prevChangeX, prevChangeY)) {
        if (get(curChangeX, curChangeY) ==
            get(prevChangeX, prevChangeY) &&
            get(curChangeX, curChangeY) != ' ') {
          nLine++;
        } else {
          nLine = 1;
          prevChangeX = curChangeX;
          prevChangeY = curChangeY;
        }

        if (nLine == _winsN) {
          _endGame = true;
          _winer = get(curChangeX, curChangeY);
          _startWinLineX = curChangeX;
          _startWinLineY = curChangeY;
          _startWinLineDerX = -derX[i];
          _startWinLineDerY = -derY[i];
        }
      }
    }
  }
}
