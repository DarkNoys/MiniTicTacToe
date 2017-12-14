#pragma once
#include "tictactoe.h"
#include <iostream>
#include <string>
#include <vector>
#include <ncurses.h>
#include <unistd.h>

#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write





class Client {
private:

  struct LogNode {
    char marker;
    char x;
    char y;

    LogNode(char m, char nx, char ny)
      :marker(m)
      ,x(nx)
      ,y(ny){
    }
  };

  struct WindowBox {
    WINDOW *win;

    int _nlines,_ncols;

    ~WindowBox() {
      delwin(win);
    }

    void init(size_t nlines, size_t ncols, size_t y0, size_t x0, bool boxed = true);

    void clear();
  };


  bool turn;
  int sock;
  char name[20];
  char enemName[20];
  char marker[1];
  char enemMarker;

  bool endGame;
  std::vector<LogNode> log;
  TicTacToe game;
  WindowBox action_log;
  WindowBox game_info;
  WindowBox map;
  int posMapX;
  int posMapY;
  WindowBox subMap;

  int currentCellX;
  int currentCellY;
  std::string address;
public:

  void erace();

  Client(std::string addr = "127.0.0.1");

  ~Client() {
    endwin();
  }

  const char X_3x3[3][3] = {{'x', ' ', 'x'},
                            {' ', 'x', ' '},
                            {'x', ' ', 'x'}};
  const char O_3x3[3][3] = {{' ', 'x', ' '},
                            {'x', ' ', 'x'},
                            {' ', 'x', ' '}};
  const char NUL_3x3[3][3] = {{' ', ' ', ' '},
                              {' ', ' ', ' '},
                              {'T', ' ', ' '}};



  void drowCell(int x, int y, const char a[3][3]);

  void drowMap();

  void changeCurrnt(int derx, int dery);

  void connectServ();

  void readName();

  void getTake();

  void drowMenu(WindowBox menu, int p);

  void drowLog();

  void drowPlayerInfo();

  void startMenu();

  void initEndGame();

  void run();

  void setTake();

  void setTakeEnem(size_t x, size_t y);
};
