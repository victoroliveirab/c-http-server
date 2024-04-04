#include "internal/thread_pool.h"
#include "internal/http.h"
#include "internal/queue.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

ThreadPool_t *create_thread_pool(Server_t * server, short size) {
  ThreadPool_t *pool = malloc(sizeof(ThreadPool_t));
  pthread_mutex_init(&pool->mutex, NULL);
  pthread_cond_init(&pool->cond, NULL);
  pool->size = size;
  pool->thread_pool = malloc(size * sizeof(pthread_t));
  pool->queue = (JobQueue_t *)queue_create();
  pool->server = server;
  return pool;
}

void read_socket(int id, char *buffer, size_t buffer_size) {
  int msg_size = 0;
  size_t bytes_read;

  while (
      (bytes_read = read(id, buffer + msg_size, buffer_size - msg_size - 1))) {
    msg_size += bytes_read;
    if (msg_size >= buffer_size || buffer[msg_size - 1] == '\n') {
      break;
    }
  }

  if (bytes_read == -1) {
    perror("Read error");
    // TODO: handle error better here
    return;
  }

  buffer[msg_size - 1] = '\0';
}

void *worker(void *arg) {
  ThreadPool_t *pool = (ThreadPool_t *)arg;
  while (1) {
    pthread_mutex_lock(&pool->mutex);
    int *client_socket = deque((Queue_t *)pool->queue);
    while (client_socket == NULL) {
      pthread_cond_wait(&pool->cond, &pool->mutex);
      client_socket = deque((Queue_t *)pool->queue);
    }

    pthread_mutex_unlock(&pool->mutex);
    int id = *client_socket;
    char buffer[4096];
    read_socket(id, buffer, 4096);
    Request_t *req = parse_http_request(buffer);
    char *response = handle_request(pool->server, req);
    send(id, response, strlen(response), 0);
    close(id);
    free(req);
    free(client_socket);
  }
  return NULL;
}
