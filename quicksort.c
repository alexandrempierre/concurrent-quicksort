#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"
#include "insertion_sort.h"
#include "lcg.h"
#include "timer.h"

#define FALSE   0
#define TRUE    1

/* This work wouldn't be possible without https://en.wikipedia.org/wiki/Quicksort#Optimizations */

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

int v_length;
int n_threads;

int insertion_threshold;

int quicksort (int, int, uint32_t *);
int median (int, int, int);
int random_idx (int, int, uint32_t *);

void *worker (void *arg) {
    uint32_t state = 1 + *(int *) arg;

    int p, lims[2], lo, hi;

    while ( !empty(to_do) || doing ) {
        /* If there are threads processing subarrays but no tasks to do, wait until
         * any task comes up or no thread is doing anything.
         * 
         * If the task queue is empty and no thread is processing any subarray we 
         * can say that the array is sorted and this thread can close itself
         */
        pthread_mutex_lock(&mutex);
        {
            while (empty(to_do) && doing) {
                pthread_cond_wait(&cond, &mutex);
            } // I find it more understandable with the braces in this specific case

            doing++;

            dequeue(to_do, lims);
            
            lo = lims[0];
            hi = lims[1];
        }
        pthread_mutex_unlock(&mutex);

        if (hi-lo < insertion_threshold) {
            insertion_sort(lo, hi, v);
            p = lo = hi;
        }
        else {
            p = quicksort(lo, hi, &state);
        }

        pthread_mutex_lock(&mutex);
        {
            if (lo < p) enqueue(lo, p, to_do);

            if (p+1 < hi) enqueue(p+1, hi, to_do);
            
            doing--;
            pthread_cond_broadcast(&cond);
        }
        pthread_mutex_unlock(&mutex);
    }

    free(arg);

    /* When there's no thread doing anything and no items waiting in the queue 
     * it's time to end the thread.
     */
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    int i, t;

    pthread_t *thread;
    int *thread_id;

    double start, finish;
    
    pthread_mutex_init(&mutex, NULL); // initialize mutual exclusion variable
    pthread_cond_init(&cond, NULL); // initialize condition variable

    /* checks whether the user supplied enough command line arguments */
    if (argc < 4) {
        fprintf(stderr, "ERROR -- not enough command line arguments\n");
        fprintf(stderr, "Try calling: %s <number of threads> <insertion sort threashold> <array length>\n", argv[0]);
        
        return 1;
    }

    sscanf(argv[1], "%d", &n_threads); // read threads number
    /* if the number of elements in an array drop to insertion_threshold or 
     * lower, the elements in the subarray will be sorted with insertion sort
     * */
    sscanf(argv[2], "%d", &insertion_threshold);
    sscanf(argv[3], "%d", &v_length); // read (to be sorted) array size

    /* validates the number of threads */
    if (n_threads < 1) n_threads = 1;

    /* allocate thread identifier array */
    if ( ( thread = (pthread_t *) malloc(n_threads * sizeof(pthread_t)) ) == NULL ) {
        fprintf(stderr, "ERROR -- malloc -- thread (pthread_t array)\n");

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

    GET_TIME(start); // I'll measure the time to sort the array

    /* add to the queue the task of sorting the whole array */
    to_do = create_queue();
    enqueue(0, v_length-1, to_do);

    doing = 0;

    /* create threads */
    for (t = 0; t < n_threads; t++) {
        thread_id = (int *) malloc(sizeof(int));
        *thread_id = t;
        if ( pthread_create(thread + t, NULL, worker, (void *) thread_id) ) {
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

    /* The time to read the array entries or check the correctness won't be 
     * measured.
     * */
    GET_TIME(finish);
    
    /* check result */
    for (i = 1; i < v_length; i++) {
        if (v[i-1] > v[i]) {
            fprintf(stderr, "ERROR -- array is not sorted\n");
            fprintf(stderr, "v[%d] = %d, v[%d] = %d\n", i-1, v[i-1], i, v[i]);

            return 5;
        }
    }

    //puts("Array is sorted!");

    printf("%d,%d,%d,%f\n", v_length, n_threads, insertion_threshold, finish-start);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    free(thread);
    free(v);
    destroy_queue(to_do);

    return 0;
}

int quicksort (int lo, int hi, uint32_t *state) {
    int pivot; 
    int i=lo-1, j=hi+1;
    int temp;

    int idx1, idx2, idx3;
    idx1 = random_idx(lo, hi, state);
    idx2 = random_idx(lo, hi, state);
    idx3 = random_idx(lo, hi, state);

    pivot = median(v[idx1], v[idx2], v[idx3]);
    
    while ( 1 ) {
        do { i++; } while( v[i] < pivot );
        do { j--; } while( v[j] > pivot );

        if ( i >= j ) { 
            return j; 
        }
        
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
}

int median (int a, int b, int c) {
    return (a >= b && a <= c) || (a >= c && a <= b) ? a : 
        (b >= a && b <= c) || (b >= c && b <= a) ? b : c;
}

int random_idx (int lo, int hi, uint32_t *state) {
    return lo + (lcg_parkmiller(state) % (hi - lo + 1));
}