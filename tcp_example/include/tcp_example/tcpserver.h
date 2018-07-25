#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <iostream>
#include <string>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

using namespace std;

class TcpServer{
private:
  char buffer[256] = {};

  int SocketListen;
  int SocketClient;
  socklen_t len;
  struct sockaddr_in addr_listen, addr_client;
  static const unsigned int buffsize = 1000;
  char mesg[buffsize];

  unsigned int port_listen;
  bool started;
  bool stopSock;

public:
  TcpServer();
  ~TcpServer();

  bool inited;

  int CreateSocket();
  int WaitConnet();
  void SendData(const string &);



  void Close();


};

#endif // TCPSERVER_H
