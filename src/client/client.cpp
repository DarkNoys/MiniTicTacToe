#include "client.h"
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


void Client::WindowBox::init(size_t nlines, size_t ncols, size_t y0, size_t x0, bool boxed) {
  win = newwin(nlines, ncols,
               y0, x0);
  if (boxed)
    box(win, 0, 0);
  wrefresh(win);
}

void Client::WindowBox::clear() {
  wclear(win);
  wrefresh(win);
}

void Client::erace() {
  Client c;

  turn = c.turn;
  sock = c.sock;
  name[20] = c.name[20];
  enemName[20] = c.enemName[20];
  marker[1] = c.marker[1];
  enemMarker = c.enemMarker;

  endGame = c.endGame;
  log = c.log;
  game = c.game;
  action_log = c.action_log;
  game_info = c.game_info;
  map = c.map;
  posMapX = c.posMapX;
  posMapY = c.posMapY;
  subMap = c.subMap;

  currentCellX = c.currentCellX;
  currentCellY = c.currentCellY;

  endGame = false;
  currentCellX = 0;
  currentCellY = 0;


  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  start_color();
  init_pair(1,  COLOR_RED, COLOR_BLUE);
  init_pair(2,  COLOR_WHITE, COLOR_BLACK);
  init_pair(3,  COLOR_WHITE, COLOR_BLACK);
  init_pair(4,  COLOR_RED, COLOR_BLACK);
  attron(COLOR_PAIR(2));

  refresh();

  game_info.init(LINES/4, COLS/3,
                 0, COLS*2/3);

  action_log.init(LINES - LINES/4, COLS/3,
                  LINES/4, COLS*2/3);

  subMap.init(LINES, COLS*2/3,
              0, 0);

  int sizeMap = 5*3+1;
  posMapY = LINES/2-sizeMap/2;
  posMapX = COLS/3-sizeMap/2;
  map.init(sizeMap, sizeMap,
           posMapY,
           posMapX,
           false);
}

Client::Client() {
  endGame = false;
  currentCellX = 0;
  currentCellY = 0;


  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  start_color();
  init_pair(1,  COLOR_RED, COLOR_BLUE);
  init_pair(2,  COLOR_WHITE, COLOR_BLACK);
  init_pair(3,  COLOR_WHITE, COLOR_BLACK);
  init_pair(4,  COLOR_RED, COLOR_BLACK);
  attron(COLOR_PAIR(2));

  refresh();

  game_info.init(LINES/4, COLS/3,
                 0, COLS*2/3);

  action_log.init(LINES - LINES/4, COLS/3,
                  LINES/4, COLS*2/3);

  subMap.init(LINES, COLS*2/3,
              0, 0);

  int sizeMap = 5*3+1;
  posMapY = LINES/2-sizeMap/2;
  posMapX = COLS/3-sizeMap/2;
  map.init(sizeMap, sizeMap,
           posMapY,
           posMapX,
           false);
}


void Client::drowCell(int x, int y, const char a[3][3]) {
  auto curr = A_NORMAL;
  mvwhline(map.win, y*4, x*4,
           '*' | curr | COLOR_PAIR(1), 5);
  mvwhline(map.win, (y+1)*4, x*4,
           '*' | curr | COLOR_PAIR(1), 5);
  mvwvline(map.win, y*4, x*4,
           '*' | curr | COLOR_PAIR(1), 5);
  mvwvline(map.win, y*4, (x+1)*4,
           '*' | curr | COLOR_PAIR(1), 5);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++)
      mvwaddch(map.win,
               y*4+j+1,
               x*4+i+1,
               a[j][i] | curr | COLOR_PAIR(3));
  }
}

void Client::drowMap() {
  for (int x = 0; x < game.getN(); x++)
    for (int y = 0; y < game.getN(); y++)
      switch (game.get(x, y)) {
      case ' ':
        drowCell(x, y, NUL_3x3);
        break;
      case 'x':
        drowCell(x, y, X_3x3);
        break;
      case 'o':
        drowCell(x, y, O_3x3);
        break;
      }

  int x = currentCellX, y = currentCellY;
  mvwhline(map.win, y*4, x*4,
           '*' | A_STANDOUT | COLOR_PAIR(1), 5);
  mvwhline(map.win, (y+1)*4, x*4,
           '*' | A_STANDOUT  | COLOR_PAIR(1), 5);
  mvwvline(map.win, y*4, x*4,
           '*' | A_STANDOUT | COLOR_PAIR(1), 5);
  mvwvline(map.win, y*4, (x+1)*4,
           '*' | A_STANDOUT | COLOR_PAIR(1), 5);

  wrefresh(map.win);
}


void Client::changeCurrnt(int derx, int dery) {
  if (game.inRange(currentCellX + derx,
                   currentCellY + dery)) {
    currentCellX += derx;
    currentCellY += dery;
  }
}

void Client::connectServ() {
  struct sockaddr_in server;

  //Create socket
  sock = socket(AF_INET , SOCK_STREAM , 0);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons( 8967 );

  //Connect to remote server
  while (0 > connect(sock, (sockaddr *)&server , sizeof(server))) {};

  write(sock, name, 20);
  read(sock, enemName, 20);
  read(sock, marker, 1);

  if (marker[0] == 'x') {
    turn = true;
  } else {
    turn = false;
  }
}

void Client::readName() {
  WindowBox nameBox;
  posMapY = LINES/2-3/2;
  posMapX = COLS/3-22/2;
  nameBox.init(3, 22,
               posMapY,
               posMapX);
  wmove(nameBox.win, 1, 1);

  echo();

  mvwgetstr(nameBox.win,1,1,name);

  noecho();
  nameBox.clear();
}

void Client::getTake() {
  char m[5];
  if (recv(sock, m, 5, 0) >= 0) {
    if (m[0] == 'w')
      endGame = true;
    setTakeEnem(m[2]-'0', m[4]-'0');
    log.push_back(LogNode(enemMarker,
                          m[2],
                          m[4]));
    turn = true;
  }
}

void Client::drowMenu(WindowBox menu, int p) {
  std::string new_game_str = "New game";
  std::string exit_str = "Exit";

  if (p == 0) {
    wattron(menu.win,COLOR_PAIR(4));
  }
  mvwaddstr(menu.win, 3, 1, new_game_str.c_str());
  wrefresh(menu.win);
  wattron(menu.win,COLOR_PAIR(2));

  if (p == 1) {
    wattron(menu.win,COLOR_PAIR(4));
  }
  mvwaddstr(menu.win, 5, 1, exit_str.c_str());
  wrefresh(menu.win);
  wattron(menu.win,COLOR_PAIR(2));
}

void Client::drowLog() {
  for(int i = 0; i < log.size(); i++) {
    mvwprintw(action_log.win, i*2+1, 2,
              "%i: %c %c %c",
              i, log[i].marker | COLOR_PAIR(4),
              log[i].x,
              log[i].y);
    wrefresh(action_log.win);
  }
}

void Client::drowPlayerInfo() {
  wmove(game_info.win, LINES/4/2, 1);

  int flag;
  if (turn) {
    wattron(game_info.win,COLOR_PAIR(4));
  }
  wprintw(game_info.win, "%c %s",
          marker[0],
          name);
  wrefresh(game_info.win);
  wattron(game_info.win, COLOR_PAIR(2));


  waddstr(game_info.win, " vs ");
  wrefresh(game_info.win);

  if (marker[0] == 'x') {
    enemMarker = 'o';
  } else {
    enemMarker = 'x';
  }

  if (!turn) {
    wattron(game_info.win, COLOR_PAIR(4));
  }
  wprintw(game_info.win, "%c %s",
          enemMarker,
          enemName);
  wrefresh(game_info.win);
  wattron(game_info.win, COLOR_PAIR(2));
}

void Client::startMenu() {
  wclear(stdscr);
  refresh();
  box(stdscr, 0, 0);
  WindowBox menu;
  posMapY = LINES/2-10/2;
  posMapX = COLS/3-9/2;
  menu.init(10, 9, posMapY, posMapX);

  int pos = 0;
  while (true) {
    drowMenu(menu, pos);
    int ch = getch();
    switch (ch) {
    case KEY_UP:
      pos = abs(pos-1) % 2;
      break;
    case KEY_DOWN:
      pos = (pos+1) % 2;
      break;
    case 'g':
      if (pos == 0) {
        erace();
        run();
        return;
      } else {
        return;
      }
    }
  }
}

void Client::initEndGame() {
  WindowBox mes;
  posMapY = LINES/2-3/2;
  posMapX = COLS/3-22/2;

  std::string end = "Win player ";

  if (marker[0] == game.getWiner()) {
    end += marker[0];
  } else {
    end += enemMarker;
  }
  mes.init(3, end.size()+2,
           posMapY,
           posMapX);

  mvwaddstr(mes.win, 1, 1, end.c_str());
  wrefresh(mes.win);

  nodelay(stdscr, FALSE);
  getch();
  nodelay(stdscr, TRUE);

  mes.clear();
  startMenu();
}

void Client::run() {
  readName();
  connectServ();
  drowPlayerInfo();
  while (true) {

    if (game.getEndGame()) {
      initEndGame();
      return;
    }

    drowLog();
    drowPlayerInfo();
    drowMap();
    if (!turn)
      getTake();
    drowMap();
    drowPlayerInfo();
    drowLog();

    if (game.getEndGame()) {
      initEndGame();
      return;
    }

    if (!turn) {
      break;
    }
    int ch = getch();
    switch (ch) {
    case KEY_UP:
      changeCurrnt(0, -1);
      break;
    case KEY_DOWN:
      changeCurrnt(0, 1);
      break;
    case KEY_LEFT:
      changeCurrnt(-1, 0);
      break;
    case KEY_RIGHT:
      changeCurrnt(1, 0);
      break;
    case 'g':
      setTake();
      break;
    }
  }
}

void Client::setTake() {
  bool tak;
  if (marker[0] == 'x')
    tak = game.TakeX(currentCellX, currentCellY);
  else
    tak = game.TakeO(currentCellX, currentCellY);

  if (!tak)
    return;

  std::string win;
  if (game.getEndGame())
    win = "w";
  else
    win = "n";

  write(sock, (win + " "
               + std::to_string(currentCellX)
               + " "
               + std::to_string(currentCellY)).c_str(),
        5);

  log.push_back(LogNode(marker[0],
                        currentCellX+'0',
                        currentCellY+'0'));
  turn = false;
}

void Client::setTakeEnem(size_t x, size_t y) {
  if (marker[0] != 'x')
    game.TakeX(x, y);
  else
    game.TakeO(x, y);
}
