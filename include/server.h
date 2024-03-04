#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>

struct Server {
  int id;
  struct sockaddr_in address;
  socklen_t address_len;
};

struct ServerConnection {
  int id;
  int status;
};

typedef struct Server Server_t;
typedef struct ServerConnection ServerConnection_t;

Server_t *server_init(int server_port, int server_backlog);
ServerConnection_t *server_receive(Server_t *server);

#endif
