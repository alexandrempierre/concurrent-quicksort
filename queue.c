#include "queue.h"

/* Allocates the queue pointer and returns the address */
t_queue *create_queue (void) {
    t_queue *queue = (t_queue *) malloc( sizeof(t_queue) );

    /* If the allocation fails, the program is terminated */
    if (queue == NULL) {
        fprintf(stderr, "ERROR -- malloc -- `queue` variable from create_queue function\n");

        exit(2);
    }

    /* An empty queue has nothing in the first and last positions */
    queue->first = NULL;
    queue->last = NULL;

    return queue;
}

/* Deallocate all nodes and the queue pointer itself. 
 * Needs the queue pointer as a parameter and returns nothing. 
 * */
void destroy_queue (t_queue *queue) {
    /* If the queue is not empty, first deallocate all its nodes. */
    if ( !empty(queue) ) destroy_nodes(queue->first);

    /* If the queue is not deallocated already deallocate it. */
    if ( queue != NULL ) {
        queue->first = NULL;
        queue->last = NULL;
        
        free(queue);
    }
}

/* Given a node destroys all the chain of nodes that starts at the
 * the pointer given as an argument. Returns nothing. */
void destroy_nodes (t_queue_node *node) {
    if (node == NULL) return; // nothing to do if there no pointers to deallocate

    /* First go to the end of the chain by stacking recursive calls... */
    if (node->next != NULL) destroy_nodes(node->next);

    /* ...then deallocate the nodes */
    free(node);
}

/* Given values and a queue, creates a node with the values and put it at the 
 * end of the queue. 
 * 
 * This is the first place where it becomes obvious that this queue 
 * implementation ir very specific to the application.
 * 
 * This function just make changes to the queue without returning
 * anything.
 * */
void enqueue (int lo, int hi, t_queue *queue) {
    t_queue_node *node = (t_queue_node *) malloc(sizeof(t_queue_node));

    /* If the allocation fails, the program is terminated */
    if (node == NULL) {
        fprintf(stderr, "ERROR -- malloc -- `node` variable from enqueue function\n");

        exit(2);
    }

    node->lo = lo;
    node->hi = hi;
    node->next = NULL;

    /* If the pointer to the last element is NULL it means the queue is empty. */
    if (queue->last == NULL)
        queue->first = node;
    else
        queue->last->next = node;
    
    /* The newly created node is set as the last node of the queue. */
    queue->last = node;
}

/* Takes the values from the first position of the queue and removes it from 
 * the queue. The function doesn't return anything because the values are 
 * stored in the `r` variable passed by reference. A pointer to the queue 
 * have to be passed to the function too.
 * */
void dequeue (t_queue *queue, int r[2]) {
    t_queue_node *first;

    /* If the queue is empty there are no values to retrieve. */
    if ( !empty(queue) ) {
        first = queue->first;
        
        r[0] = first->lo; 
        r[1] = first->hi;
        
        queue->first = first->next;
        
        /* If the last element is taken from the queue, it gets empty and 
         * should have first and last elements set to NULL. */
        if ( queue->first == NULL ) queue->last = NULL;

        free(first); // the node can be deallocated
    }
}

/* Given a queue returns whether or not it is empty. */
bool empty (t_queue *queue) {
    /* An unallocated queue is considered empty just like a allocated empty 
     * queue. 
     * */
    return ( queue == NULL || queue->first == NULL );
}

/* Given a queue and a string containing the name of the queue, prints its 
 * content in the format: 
 * 
 * <name>: <first value> => <second value> => ... => <last value>
 * 
 * The contents of the queue is printed to the standard output and the 
 * function returns nothing.
 * */
void print_queue (t_queue *queue, const char *queue_name) {
    /* Print 'queue' if NULL is passed where it should be the queue name */
    if ( queue_name == NULL )
        printf("queue: ");
    else 
        printf("%s: ", queue_name);

    /* Prints the values in each node */
    for (t_queue_node *n = queue->first; n != NULL; n = n->next) {
        print_queue_node(n);

        /* if the node isn't the last, print the separator */
        if ( n->next != NULL ) printf(" => ");
    }

    printf("\n");
}

/* Given a pointer to a node print its contents */
void print_queue_node (t_queue_node *node) {
    printf("[%d..%d]", node->lo, node->hi);
}
