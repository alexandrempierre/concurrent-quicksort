#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>

typedef int bool;

typedef struct queue_node {
    size_t lo, hi;
    struct queue_node *next;
} t_queue_node;

typedef struct queue {
    struct queue_node *first, *last;
} t_queue;

t_queue *create_queue (void);

void enqueue (int lo, int hi, t_queue *queue);

void dequeue (t_queue *queue, size_t lims[2]);

bool empty (t_queue *queue);

void print_queue (t_queue *queue, const char *queue_name);

void print_queue_node (t_queue_node *node);

#endif