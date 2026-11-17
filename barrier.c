/* barrier.c: reusable condition-variable barrier (skeleton)
 *
 * Requirements:
 *  - Use the mutex, condition variable, and generation counter in barrier_t.
 *  - The last thread to arrive resets count, advances generation,
 *    and broadcasts to wake the others.
 *  - Waiting threads must loop on the generation counter to handle
 *    spurious wakeups.
 *  - The barrier must be reusable across many rounds.
 */
#include <pthread.h>
#include "barrier.h"

int barrier_init(barrier_t *b, int num_threads) {
    (void)b;
    (void)num_threads;
    /* TODO: set num_threads, count = 0, generation = 0,
     * initialize mutex and condition variable.
     * Return 0 on success, nonzero on failure.
     */
    return 0;
}

void barrier_wait(barrier_t *b) {
    (void)b;
    /* TODO:
     *  1. Lock the mutex.
     *  2. Record the current generation.
     *  3. Increment count.
     *  4. If count == num_threads: reset count, advance generation,
     *     broadcast the condition variable.
     *     Otherwise: loop on pthread_cond_wait while the generation is unchanged.
     *  5. Unlock the mutex.
     */
}

void barrier_destroy(barrier_t *b) {
    (void)b;
    /* TODO: destroy the condition variable and mutex */
}
