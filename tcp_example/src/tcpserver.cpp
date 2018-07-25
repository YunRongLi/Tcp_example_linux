#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tcp_example/tcpserver.h"

#define PortNum 8845

TcpServer::TcpServer(){
  port_listen = PortNum;
}

int TcpServer::CreateSocket(){
  int value = 0;

  SocketListen = socket(AF_INET, SOCK_STREAM, 0);

  if (SocketListen < 0){
    std::cout << "[Error] " << errno << " Open Socket." << std::endl;
    return -1;
  }

  addr_listen.sin_family = AF_INET;
  addr_listen.sin_port = htons(port_listen);
  addr_listen.sin_addr.s_addr = INADDR_ANY;

  value = bind(SocketListen, (struct sockaddr*)&addr_listen, sizeof(addr_listen));
//  value = connect(SocketListen, (struct sockaddr*)&SocketListen, sizeof(SocketListen));

  if (value < 0){
    std::cout << "[Error] " << errno << " Bind Socket." << std::endl;
    return -1;
  }

  std::cout << "[Info] binding..." << std::endl;

  value = listen(SocketListen, 5);

  if (value < 0){
    std::cout << "[Error] " << errno << " Listen Socket." << std::endl;
    return -1;
  }

  std::cout << "[Info] listening Socket..." << std::endl;

  return 0;
}

int TcpServer::WaitConnet(){
  len = sizeof(addr_client);

  SocketClient = accept(SocketListen, (struct sockaddr*)&addr_client, &len);
  if (SocketClient < 0){
      std::cout << "[Error] " << errno << "Accept Connect." << std::endl;
  }

  char *addr_str = inet_ntoa(addr_client.sin_addr);
  std::cout << "[Info] Receive connection from " << addr_str << std::endl;


}

void TcpServer::SendData(const string &data){
  int value = send(SocketClient, data.c_str(), sizeof(data.c_str()), 0);
  if (value < 0){
      std::cout << "[Error] "  << errno << "Send Data." << std::endl;
  }
}


void TcpServer::Close(){
  close(SocketClient);
  close(SocketListen);
}

int main()
{
  int sock0;
  struct sockaddr_in addr;
  struct sockaddr_in client;
  socklen_t len;
  int SocketClient;

  /* 製作 socket */
  sock0 = socket(AF_INET, SOCK_STREAM, 0); // AF_INET:Interent Address

  /* 設定 socket */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock0, (struct sockaddr*)&addr, sizeof(addr));
  printf("\t[Info] binding...\n");

  /* 設定成等待來自 TCP 用戶端的連線要求狀態 */
  listen(sock0, 5);
  printf("\t[Info] listening...\n");

  /* 接受來自 TCP 用戶端地連線要求*/
  printf("\t[Info] wait for connection...\n");
  len = sizeof(client);
  SocketClient = accept(sock0, (struct sockaddr *)&client, &len);
  printf("\t[Info] Testing...\n");
  char *paddr_str = inet_ntoa(client.sin_addr);
  printf("\t[Info] Receive connection from %s...\n", paddr_str);

  /* 傳送 5 個字元 */
  printf("\t[Info] Say hello back...\n");
  write(SocketClient, "HELLO\n", 6);

  /* 結束 TCP 對話 */
  printf("\t[Info] Close client connection...\n");
  close(SocketClient);

  /* 結束 listen 的 socket */
  printf("\t[Info] Close self connection...\n");
  close(sock0);
  return 0;
}
