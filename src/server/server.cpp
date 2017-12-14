#include <iostream>
#include <stdio.h>
#include <set>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include<pthread.h> //for threading , link with lpthread


struct Attr {
  int client1, client2;
  char name1[20], name2[20];
};

void *play(void* attr);

class Server {
private:


  int mainSocket, client1Socket, client2Socket;
  sockaddr_in server, client1, client2;

public:

  Server() {
    mainSocket = socket(AF_INET , SOCK_STREAM , 0);


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8967 );

    bind(mainSocket, (sockaddr *)&server, sizeof(server));
  }



  void run() {
    listen(mainSocket, 20);

    Attr *attr;

    while (true) {
      int c = sizeof(sockaddr_in);
      client1Socket = accept(mainSocket, (sockaddr *)&client1, (socklen_t*)&c);
      client2Socket = accept(mainSocket, (sockaddr *)&client1, (socklen_t*)&c);

      attr = new Attr;
      attr->client1 = client1Socket;
      attr->client2 = client2Socket;
      read(client1Socket, attr->name1, 20);
      read(client2Socket, attr->name2, 20);

      pthread_t sniffer_thread;
      pthread_create(&sniffer_thread, NULL, play, (void*) attr);
    }
    close(mainSocket);
  }




};

void *play(void *attr) {
  Attr *a = (Attr *)attr;
  // отправить информацию о символе игрока
  int client1 = a->client1;
  int client2 = a->client2;
  try {

  char massage[5];

  if (write(client1, a->name2, 20) < 0){
    throw;
  }
  if (write(client2, a->name1, 20) < 0){
    throw;
  }

  if (write(client1, "x", 1) < 0){
    throw;
  }
  if (write(client2, "o", 1) < 0){
    throw;
  }

  while (true) {
    if (read(client1, massage, 5) < 0){
      throw;
    }
    puts(massage);
    if (write(client2, massage, 5) < 0){
      throw;
    }

    if (massage[1] == 'w') {
      break;
    }



    if (read(client2, massage, 5) < 0){
      throw;
    }
    if (write(client1, massage, 5) < 0){
      throw;
    }

    if (massage[0] == 'w') {
      break;
    }
  }

  //Отправить игрокам информацию о победителе и разоравать сессию
  close(client1);
  close(client2);
  delete a;
  } catch (...) {
    delete a;
  }
}

int main(int argc , char *argv[])
{
  Server server;
  server.run();
  return 0;
}
