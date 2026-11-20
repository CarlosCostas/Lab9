/* barrier.c: reusable condition-variable barrier (skeleton)
 *
- Requirements:
-  - Use the mutex, condition variable, and generation counter in barrier_t.
-  - The last thread to arrive resets count, advances generation,
-    and broadcasts to wake the others.
-  - Waiting threads must loop on the generation counter to handle
-    spurious wakeups.
-  - The barrier must be reusable across many rounds.
 */
#include <pthread.h>
#include "barrier.h"

int barrier_init(barrier_t *b, int num_threads) {
    (void)b;
    (void)num_threads;
    /* TODO: set num_threads, count = 0, generation = 0,
- initialize mutex and condition variable.
- Return 0 on success, nonzero on failure.
     */
    if (b == NULL || num_threads <= 0) {
        return -1;
    }

    b->num_threads = num_threads;
    b->count = 0;
    b->generation = 0;

    int rc = pthread_mutex_init(&b->mutex, NULL);
    if (rc != 0) {
        return rc;
    }

    rc = pthread_cond_init(&b->cv, NULL);
    if (rc != 0) {
        pthread_mutex_destroy(&b->mutex);
        return rc;
    }

    return 0;
}

void barrier_wait(barrier_t *b) {
    (void)b;
    /* TODO:
-  1. Lock the mutex.
-  2. Record the current generation.
-  3. Increment count.
-  4. If count == num_threads: reset count, advance generation,
-     broadcast the condition variable.
-     Otherwise: loop on pthread_cond_wait while the generation is unchanged.
-  5. Unlock the mutex.
     */
    pthread_mutex_lock(&b->mutex);

    unsigned long gen = b->generation;
    b->count++;

    if (b->count == b->num_threads) {
        b->count = 0;
        b->generation++;
        pthread_cond_broadcast(&b->cv);
    } else {
        while (gen == b->generation) {
            pthread_cond_wait(&b->cv, &b->mutex);
        }
    }

    pthread_mutex_unlock(&b->mutex);
}

void barrier_destroy(barrier_t *b) {
    (void)b;
    /* TODO: destroy the condition variable and mutex */
    pthread_cond_destroy(&b->cv);
    pthread_mutex_destroy(&b->mutex);
}