#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "../../include/server.h"

#include <pthread.h>

#define THREAD_POOL_SIZE 10

struct JobNode {
  int *client_socket;
  struct JobNode *next;
};

struct JobQueue {
  struct JobNode *head;
  struct JobNode *tail;
};

struct ThreadPool {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  pthread_t *thread_pool;
  struct JobQueue *queue;
  Server_t *server;
  size_t size;
};

typedef struct JobNode JobNode_t;
typedef struct JobQueue JobQueue_t;
typedef struct ThreadPool ThreadPool_t;

ThreadPool_t *create_thread_pool(Server_t * server, short size);
void *worker(void *arg);

#endif
