/* Benchmark driver. Usage: ./bench <threads> <read|write> <ops_per_thread>
 * read  workload: 90% Member, 10% Insert/Delete
 * write workload: 10% Member, 90% Insert/Delete
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "list.h"

#define KEY_RANGE 1000

static int read_heavy;
static long ops_per_thread;

static void *worker(void *arg) {
    unsigned int seed = (unsigned int)(size_t)arg;
    for (long i = 0; i < ops_per_thread; i++) {
        int key = rand_r(&seed) % KEY_RANGE;
        int roll = rand_r(&seed) % 100;
        int member_pct = read_heavy ? 90 : 10;
        if (roll < member_pct) {
            Member(key);
        } else if (roll % 2 == 0) {
            Insert(key);
        } else {
            Delete(key);
        }
    }
    return NULL;
}

static double now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <threads> <read|write> <ops_per_thread>\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    int threads = atoi(argv[1]);
    read_heavy = (strcmp(argv[2], "read") == 0);
    ops_per_thread = atol(argv[3]);
    if (threads < 1 || ops_per_thread < 1) {
        fprintf(stderr, "threads and ops_per_thread must be positive\n");
        return EXIT_FAILURE;
    }

    list_init();
    for (int k = 0; k < KEY_RANGE; k += 2) {
        Insert(k);   /* pre-fill about half the key range */
    }

    pthread_t *th = malloc(sizeof(pthread_t) * threads);
    if (!th) {
        return EXIT_FAILURE;
    }

    double start = now_sec();
    for (int i = 0; i < threads; i++) {
        pthread_create(&th[i], NULL, worker, (void *)(size_t)(i + 1));
    }
    for (int i = 0; i < threads; i++) {
        pthread_join(th[i], NULL);
    }
    double elapsed = now_sec() - start;

    long total = (long)threads * ops_per_thread;
    printf("threads=%d workload=%s ops=%ld time=%.4fs throughput=%.0f ops/sec\n",
           threads, argv[2], total, elapsed, total / elapsed);

    free(th);
    free_list();
    return EXIT_SUCCESS;
}
