/* Sample stress test. This is NOT the full grading test.
 * Phase 1: concurrent inserts on disjoint ranges, then verify.
 * Phase 2: barrier rounds; each round checks that all threads arrived
 *          before any thread was released.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"
#include "barrier.h"

#define NUM_THREADS     4
#define VALS_PER_THREAD 500
#define BARRIER_ROUNDS  200

static barrier_t bar;

/* arrivals[r] counts threads that have reached barrier round r.
 * Protected by arrival_lock. */
static int arrivals[BARRIER_ROUNDS];
static pthread_mutex_t arrival_lock = PTHREAD_MUTEX_INITIALIZER;

static int barrier_errors = 0;
static pthread_mutex_t err_lock = PTHREAD_MUTEX_INITIALIZER;

static void record_barrier_error(void) {
    pthread_mutex_lock(&err_lock);
    barrier_errors++;
    pthread_mutex_unlock(&err_lock);
}

static void *insert_worker(void *arg) {
    long id = (long)arg;
    for (int i = 0; i < VALS_PER_THREAD; i++) {
        int v = (int)(id * VALS_PER_THREAD + i);
        Insert(v);
    }
    /* Delete the odd values this thread inserted */
    for (int i = 1; i < VALS_PER_THREAD; i += 2) {
        int v = (int)(id * VALS_PER_THREAD + i);
        Delete(v);
    }
    return NULL;
}

static void *barrier_worker(void *arg) {
    (void)arg;
    for (int r = 0; r < BARRIER_ROUNDS; r++) {
        /* Announce arrival for this round before waiting. */
        pthread_mutex_lock(&arrival_lock);
        arrivals[r]++;
        pthread_mutex_unlock(&arrival_lock);

        barrier_wait(&bar);

        /* If the barrier released us early, some thread has not
         * yet arrived for round r. */
        pthread_mutex_lock(&arrival_lock);
        int seen = arrivals[r];
        pthread_mutex_unlock(&arrival_lock);

        if (seen != NUM_THREADS) {
            record_barrier_error();
        }
    }
    return NULL;
}

static int run_list_phase(void) {
    pthread_t th[NUM_THREADS];
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&th[i], NULL, insert_worker, (void *)i);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(th[i], NULL);
    }

    int errors = 0;
    for (int id = 0; id < NUM_THREADS; id++) {
        for (int i = 0; i < VALS_PER_THREAD; i++) {
            int v = id * VALS_PER_THREAD + i;
            int expected = (i % 2 == 0);   /* even values remain */
            if (Member(v) != expected) {
                errors++;
            }
        }
    }
    printf("List phase: %s (%d errors)\n", errors ? "FAIL" : "PASS", errors);
    return errors;
}

static int run_barrier_phase(void) {
    if (barrier_init(&bar, NUM_THREADS) != 0) {
        fprintf(stderr, "barrier_init failed\n");
        return 1;
    }

    pthread_t th[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&th[i], NULL, barrier_worker, NULL) != 0) {
            fprintf(stderr, "pthread_create failed\n");
            barrier_destroy(&bar);
            return 1;
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(th[i], NULL);
    }
    barrier_destroy(&bar);

    int errors = barrier_errors;
    printf("Barrier phase: %s (%d rounds, %d threads, %d early-release errors)\n",
           errors ? "FAIL" : "PASS", BARRIER_ROUNDS, NUM_THREADS, errors);
    return errors;
}

int main(void) {
    list_init();
    int errors = run_list_phase();
    free_list();

    errors += run_barrier_phase();

    if (errors) {
        printf("Sample stress test FAILED\n");
        return EXIT_FAILURE;
    }
    printf("Sample stress test passed\n");
    return EXIT_SUCCESS;
}