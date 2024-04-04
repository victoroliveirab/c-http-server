#include "../include/router.h"
#include "../include/server.h"
#include "internal/log.h"
#include "internal/queue.h"
#include "internal/thread_pool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 6969
#define SERVER_BACKLOG 150

long long request_id = 0;

void print_routes(Router_t *router) {
  printf("+------------------------+\n");
  printf("| Registered Routes:\n");
  char **routes = get_registered_routes(router);
  int i = 0;
  char *zero = "0";
  while (routes[i]) {
    printf("| %s%d- %s\n", i + 1 < 10 ? zero : "", i + 1, routes[i]);
    ++i;
  }
  printf("+------------------------+\n");
}

char *home_handler() {
  return "HTTP/1.1 200 OK\r\n"
         "Content-Type: text/html\r\n\r\n"
         "<html><body><h1>Hello from home page!</h1></body></html>\r\n";
}

char *posts_handler() {
  return "HTTP/1.1 200 OK\r\n"
         "Content-Type: text/html\r\n\r\n"
         "<html><body><h1>Posts</h1><ul><li><a href='/posts/1'>Post "
         "#1</a></li></ul></body></html>\r\n";
}

char *post_by_id(char *id) {
  return "HTTP/1.1 200 OK\r\n"
         "Content-Type: text/html\r\n\r\n"
         "<html><body><h1>Post #1</h1><p>First line</p><p>Second line</p>"
         "</body></html>\r\n";
}

int main() {
  Server_t *server = server_init(PORT, SERVER_BACKLOG);

  ThreadPool_t *pool = create_thread_pool(server, 10);
  for (int i = 0; i < 10; ++i) {
    pthread_create(&pool->thread_pool[i], NULL, worker, (void *)pool);
  }

  log_message("Server listening on port %d...\n", PORT);

  register_route(server->router, GET, "/home/", (void *)home_handler);
  register_route(server->router, GET, "/posts/", (void *)posts_handler);
  register_route(server->router, GET, "/posts/:id/", (void *)post_by_id);
  print_routes(server->router);

  while (1) {
    ServerConnection_t *connection = server_receive(server);
    if (!connection->status) {
      log_message("Acceptance failed\n");
      perror("Acceptance failed");
      free(connection);
      continue;
    }
    pthread_mutex_lock(&pool->mutex);
    enqueue((Queue_t *)pool->queue, (void *)&connection->id, sizeof(int));
    // log_message("N=%lld: Client ID enqueued: %d", ++request_id, connection->id);
    // log_message("Current queue size: %d", ((Queue_t *)pool->queue)->size);
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
  }

  close(server->id);
  return 0;
}
