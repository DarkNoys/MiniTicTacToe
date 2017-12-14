#pragma once
#include <vector>

class TicTacToe {

private:

  //Карта
  std::vector<std::vector<char>> _map;
  int _winsN;
  int _N;

  // 1 - игра закончена
  // 0 - игра не закончена
  bool _endGame;
  short _winer;
  int _startWinLineX;
  int _startWinLineY;
  int _startWinLineDerX;
  int _startWinLineDerY;


  bool insert(char type, size_t x, size_t y);

  void findWinArround(size_t x, size_t y);

public:

  bool inRange(size_t x, size_t y) {
    return (x < _N) && (x >= 0) &&
      (y < _N) && ( y>= 0);
  }

  TicTacToe(int winsN = 3, int N = 3)
    :_winsN(winsN)
    ,_N(N)
    ,_map(std::vector<std::vector<char>>(N,
                                        std::vector<char>(N, ' ')))
    ,_endGame(false) {
  }

  bool TakeX(size_t x, size_t y) {
    return insert('x', x, y);
  }

  bool TakeO(size_t x, size_t y){
    return insert('o', x, y);
  }

  char get(size_t x, size_t y) {
    return _map[x][y];
  }

  char getWiner() {
    return _winer;
  }

  bool getEndGame() {
    return _endGame;
  }

  int getN() {
    return _N;
  }
};
