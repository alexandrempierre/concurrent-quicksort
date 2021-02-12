#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

#define FALSE   0
#define TRUE    1

/* `todo` vai guardar os pares `min_index` e `max_index` das sublistas que já estão
 * liberadas para ser ordenadas 
 * 
 * `doing` vai guardar a quantidade de tarefas sendo feitas no momento
 * 
 * o programa acaba quando `todo` estiver vazia e `doing` for igual a zero, significando
 * que, além de não ter nenhuma tarefa pendente, não há tarefas rodando que podem 
 * alimentar a fila `todo`
 * */
t_queue *to_do;
int doing; // variable to store how many threads are active
int *v;

pthread_mutex_t mutex;
pthread_cond_t cond;

size_t v_length;
int n_threads;

size_t quicksort (size_t, size_t, size_t);

void *worker (void *arg) {
    size_t *id = (size_t *) arg;

    size_t p, lims[2], lo, hi;
    
    // printf("thread %zu começando...\n", *id);

    while ( !empty(to_do) || doing ) {
        /* If there are threads processing sublists but no tasks to do, wait until
         * any task comes up or no thread is doing anything.
         * 
         * If the task queue is empty and no thread is processing any sublist we 
         * can say that the array is sorted and this thread can close itself
         */
        pthread_mutex_lock(&mutex);
        {
            while (empty(to_do) && doing) {
                // printf("thread %zu se bloqueou\n", *id);
                pthread_cond_wait(&cond, &mutex);
                // printf("thread %zu foi desbloqueada\n", *id);
            }

            doing++;

            // printf("thread %zu desenfileirando valores\n", *id);
            dequeue(to_do, lims);
            
            lo = lims[0];
            hi = lims[1];
            // printf("thread %zu indo começar quicksort...\n", *id);
        }
        pthread_mutex_unlock(&mutex);

        p = quicksort(lo, hi, *id);

        pthread_mutex_lock(&mutex);
        {
            // printf("thread %zu enfileirando próximos valores (se houver)\n", *id);
            if (lo < p) enqueue(lo, p, to_do);

            if (p+1 < hi) enqueue(p+1, hi, to_do);
            
            doing--;
            pthread_cond_broadcast(&cond);
            // printf("thread %zu fez um broadcast\n", *id);
        }
        pthread_mutex_unlock(&mutex);
    }

    // printf("thread %zu acabou, doing = %d\n", *id, doing);

    /* When there's no thread doing anything and no items waiting in the queue 
     * it's time to end the thread.
     */
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    size_t i, t;

    pthread_t *thread;
    int *thread_id;
    
    pthread_mutex_init(&mutex, NULL); // initialize mutual exclusion variable
    pthread_cond_init(&cond, NULL); // initialize condition variable

    /* checks whether the user supplied enough command line arguments */
    if (argc < 3) {
        fprintf(stderr, "ERROR -- not enough command line arguments\n");
        fprintf(stderr, "Try calling: %s <number of threads> <array length>\n", argv[0]);
        
        return 1;
    }

    sscanf(argv[1], "%d", &n_threads); // read threads number
    sscanf(argv[2], "%zu", &v_length); // read (to be sorted) array size

    /* validates the number of threads */
    if (n_threads < 1) n_threads = 1;

    /* allocate thread identifier array */
    if ( ( thread = (pthread_t *) malloc(n_threads * sizeof(pthread_t)) ) == NULL ) {
        fprintf(stderr, "ERROR -- malloc -- thread (pthread_t array)\n");

        return 2;
    }

    /* allocate thread readable identifier array */
    if ( ( thread_id = (int *) malloc(n_threads * sizeof(int)) ) == NULL ) {
        fprintf(stderr, "ERROR -- malloc -- thread_id (size_t array)\n");

        return 2;
    }

    /* allocate the array to be sorted */
    if ( ( v = (int *) malloc(v_length * sizeof(int)) ) == NULL ) {
        fprintf(stderr, "ERROR -- malloc -- v (unordered array)\n");

        return 2;
    }

    /* read array entries */
    for (i = 0; i < v_length; i++) {
        scanf("%d", v+i);
    }

    /* add to the queue the task of sorting the whole array */
    to_do = create_queue();
    enqueue(0, v_length-1, to_do);

    doing = 0;

    /* create threads */
    for (t = 0; t < n_threads; t++) {
        thread_id[t] = t;
        if ( pthread_create(thread + t, NULL, worker, (void *) (thread_id + t)) ) {
        // if ( pthread_create(thread + t, NULL, worker, NULL) ) {
            fprintf(stderr, "ERROR -- pthread_create\n");

            return 3;
        }
    }

    /* wait for the threads to end */
    for (t = 0; t < n_threads; t++) {
        if ( pthread_join(thread[t], NULL) ) {
            fprintf(stderr, "ERROR -- pthread_join\n");

            return 4;
        }
    }
    
    /* check result */
    for (i = 1; i < v_length; i++) {
        if (v[i-1] > v[i]) {
            fprintf(stderr, "ERROR -- array is not sorted\n");
            fprintf(stderr, "v[%zu] = %d, v[%zu] = %d\n", i-1, v[i-1], i, v[i]);

            return 5;
        }
    }

    /* print result */
    // printf("Sorted array: {");    
    // for (i = 0; i < v_length; i++){
    //     printf("%d%s", v[i], i < v_length-1 ? ", " : "");
    // }
    // printf("}\n");

    puts("Array is sorted!");
    
    return 0;
}

size_t quicksort (size_t lo, size_t hi, size_t id) {
    size_t pivot = v[lo + (hi-lo)/2]; // this is an overflow resistant version of (lo + hi)/2
    size_t i=lo-1, j=hi+1;
    int temp;
    
    // printf("thread %zu entrou no quicksort\n", id);
    while ( 1 ) {
        do { i++; } while( v[i] < pivot );
        do { j--; } while( v[j] > pivot );

        if ( i >= j ) { 
            // printf("thread %zu saiu do quicksort\n", id);
            return j; 
        }
        
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
}