#include "queue.h"

t_queue *create_queue (void) {
    t_queue *queue = (t_queue *) malloc( sizeof(t_queue) );

    if (queue == NULL) {
        fprintf(stderr, "ERROR -- malloc -- `queue` variable from create_queue function\n");

        exit(2);
    }

    queue->first = NULL;
    queue->last = NULL;

    return queue;
}

void enqueue (int lo, int hi, t_queue *queue) {
    t_queue_node *node = (t_queue_node *) malloc(sizeof(t_queue_node));

    if (node == NULL) {
        fprintf(stderr, "ERROR -- malloc -- `node` variable from enqueue function\n");

        exit(2);
    }

    node->lo = lo;
    node->hi = hi;
    node->next = NULL;

    if (queue->last == NULL)
        queue->first = node;
    else
        queue->last->next = node;
    
    queue->last = node;
}

void dequeue (t_queue *queue, size_t r[2]) {
    t_queue_node *first;

    if ( !empty(queue) ) {
        first = queue->first;
        
        r[0] = first->lo; 
        r[1] = first->hi;
        
        queue->first = first->next;
        
        if ( queue->first == NULL ) queue->last = NULL;

        free(first);
    }

bool empty (t_queue *queue) {
    return ( queue == NULL || queue->first == NULL );
}

void print_queue (t_queue *queue, const char *queue_name) {
    if ( queue_name == NULL )
        printf("queue: ");
    else 
        printf("%s: ", queue_name);

    for (t_queue_node *n = queue->first; n != NULL; n = n->next) {
        print_queue_node(n);

        if ( n->next != NULL ) printf(" => ");
    }

    printf("\n");
}

void print_queue_node (t_queue_node *node) {
    printf("[%ld..%ld]", node->lo, node->hi);
}
