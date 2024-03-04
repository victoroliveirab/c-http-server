#include "internal/queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Queue_t* queue_create() {
  Queue_t* queue = malloc(sizeof(Queue_t));
  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;
  return queue;
}

void enqueue(Queue_t *queue, void *element, size_t element_size) {
  QueueNode_t *node = malloc(sizeof(QueueNode_t));
  if (node == NULL) {
    printf("Allocation failed\n");
    return;
  }
  node->next = NULL;
  node->element = malloc(element_size);
  memcpy(node->element, element, element_size);

  if (queue->head == NULL) {
    queue->head = node;
  } else {
    queue->tail->next = node;
  }
  queue->tail = node;
  queue->size = queue->size + 1;
}

void *deque(Queue_t *queue) {
  if (queue->head == NULL) {
    return NULL;
  }

  QueueNode_t *head = queue->head;
  queue->head = head->next;
  void *value = head->element;
  free(head);
  queue->size = queue->size - 1;
  return value;
}
