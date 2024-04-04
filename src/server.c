#include "../include/server.h"

#include <stdio.h>
#include <stdlib.h>

Server_t *server_init(int server_port, int server_backlog) {
  Server_t *server = malloc(sizeof(Server_t));
  if ((server->id = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  server->address.sin_family = AF_INET;
  server->address.sin_addr.s_addr = INADDR_ANY;
  server->address.sin_port = htons(server_port);
  server->address_len = sizeof(struct sockaddr_in);

  if (bind(server->id, (struct sockaddr *)&server->address,
           server->address_len) == -1) {
    perror("Binding failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server->id, server_backlog) == -1) {
    perror("Listening failed");
    exit(EXIT_FAILURE);
  }

  server->router = create_router();

  return server;
}

ServerConnection_t *server_receive(Server_t *server) {
  ServerConnection_t *connection = malloc(sizeof(ServerConnection_t));
  int id = accept(server->id, (struct sockaddr *)&server->address,
                  &server->address_len);
  if (id == -1) {
    connection->status = 0;
  } else {
    connection->status = 1;
  }
  connection->id = id;
  return connection;
}
