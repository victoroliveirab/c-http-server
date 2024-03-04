#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

struct QueueNode {
  void *element;
  struct QueueNode *next;
};

struct Queue {
  struct QueueNode *head;
  struct QueueNode *tail;
  int size;
};

typedef struct QueueNode QueueNode_t;
typedef struct Queue Queue_t;

Queue_t* queue_create();
void enqueue(Queue_t *queue, void *element, size_t element_size);
void *deque(Queue_t *queue);

#endif
