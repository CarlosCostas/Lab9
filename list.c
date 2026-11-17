/* list.c: rwlock-protected sorted linked list (skeleton)
 *
 * Requirements:
 *  - Global head pointer and global pthread_rwlock_t.
 *  - Member uses a READ lock; Insert and Delete use a WRITE lock.
 *  - List stays strictly ascending with no duplicates.
 *  - Release every lock on every return path.
 *  - free_list must release every node.
 */
#include <stdlib.h>
#include <pthread.h>
#include "list.h"

typedef struct node {
    int value;
    struct node *next;
} node_t;

/* TODO: global head pointer (initialize to NULL in list_init) */
static node_t *head = NULL;

/* TODO: global read-write lock (initialize in list_init, destroy in free_list) */
static pthread_rwlock_t list_lock;

void list_init(void) {
    /* TODO: initialize head and list_lock */
}

int Member(int value) {
    (void)value;
    /* TODO: acquire read lock, search the list, release read lock on every path */
    return 0;
}

int Insert(int value) {
    (void)value;
    /* TODO: acquire write lock, insert in sorted position, reject duplicates,
     * release write lock on every path. Return 1 if inserted, 0 if duplicate.
     */
    return 0;
}

int Delete(int value) {
    (void)value;
    /* TODO: acquire write lock, unlink the node if found, free it,
     * release write lock on every path. Return 1 if deleted, 0 if not found.
     */
    return 0;
}

void free_list(void) {
    /* TODO: free every node, reset head to NULL, destroy list_lock */
}
