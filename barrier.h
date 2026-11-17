#ifndef BARRIER_H
#define BARRIER_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t  cv;
    int             num_threads;  /* number of threads that must arrive */
    int             count;        /* threads arrived in the current round */
    unsigned long   generation;   /* incremented each time a round completes */
} barrier_t;

/* Initialize the barrier for num_threads participants. Returns 0 on success. */
int barrier_init(barrier_t *b, int num_threads);

/* Block until num_threads threads have called barrier_wait for this round.
 * Safe to reuse across many rounds.
 */
void barrier_wait(barrier_t *b);

/* Destroy the mutex and condition variable. */
void barrier_destroy(barrier_t *b);

#endif
