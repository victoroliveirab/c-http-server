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

int main() {
  Server_t *server = server_init(PORT, SERVER_BACKLOG);
  Queue_t *connection_queue = queue_create();

  ThreadPool_t *pool = create_thread_pool(10, (JobQueue_t *)connection_queue);
  for (int i = 0; i < 10; ++i) {
    pthread_create(&pool->thread_pool[i], NULL, worker, (void *)pool);
  }

  printf("Server listening on port %d...\n", PORT);
  fflush(stdout);

  while (1) {
    ServerConnection_t *connection = server_receive(server);
    if (!connection->status) {
      printf("Acceptance failed\n");
      perror("Acceptance failed");
      free(connection);
      continue;
    }
    printf("Enqueuing client %d\n", connection->id);

    pthread_mutex_lock(&pool->mutex);
    enqueue(connection_queue, (void *)&connection->id, sizeof(int));
    char msg[256];
    snprintf(msg, sizeof(msg), "N=%lld: Client ID enqueued: %d", ++request_id, connection->id);
    log_message(msg);
    snprintf(msg, sizeof(msg), "Current queue size: %d", connection_queue->size);
    log_message(msg);
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
  }

  close(server->id);
  return 0;
}
