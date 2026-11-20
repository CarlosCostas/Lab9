/* list.c: rwlock-protected sorted linked list (skeleton)
 *
- Requirements:
-  - Global head pointer and global pthread_rwlock_t.
-  - Member uses a READ lock; Insert and Delete use a WRITE lock.
-  - List stays strictly ascending with no duplicates.
-  - Release every lock on every return path.
-  - free_list must release every node.
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
    head = NULL;
    pthread_rwlock_init(&list_lock, NULL);
}

int Member(int value) {
    (void)value;
    /* TODO: acquire read lock, search the list, release read lock on every path */
    pthread_rwlock_rdlock(&list_lock);

    node_t *curr = head;
    while (curr != NULL && curr->value < value) {
        curr = curr->next;
    }

    int found = (curr != NULL && curr->value == value) ? 1 : 0;
    pthread_rwlock_unlock(&list_lock);
    return found;
}

int Insert(int value) {
    (void)value;
    /* TODO: acquire write lock, insert in sorted position, reject duplicates,
- release write lock on every path. Return 1 if inserted, 0 if duplicate.
     */
    pthread_rwlock_wrlock(&list_lock);

    node_t *curr = head;
    node_t *pred = NULL;

    while (curr != NULL && curr->value < value) {
        pred = curr;
        curr = curr->next;
    }

    if (curr != NULL && curr->value == value) {
        pthread_rwlock_unlock(&list_lock);
        return 0;
    }

    node_t *temp = malloc(sizeof(node_t));
    if (temp == NULL) {
        pthread_rwlock_unlock(&list_lock);
        return 0;
    }
    temp->value = value;
    temp->next = curr;

    if (pred == NULL) {
        head = temp;
    } else {
        pred->next = temp;
    }

    pthread_rwlock_unlock(&list_lock);
    return 1;
}

int Delete(int value) {
    (void)value;
    /* TODO: acquire write lock, unlink the node if found, free it,
- release write lock on every path. Return 1 if deleted, 0 if not found.
     */
    pthread_rwlock_wrlock(&list_lock);

    node_t *curr = head;
    node_t *pred = NULL;

    while (curr != NULL && curr->value < value) {
        pred = curr;
        curr = curr->next;
    }

    if (curr == NULL || curr->value != value) {
        pthread_rwlock_unlock(&list_lock);
        return 0;
    }

    if (pred == NULL) {
        head = curr->next;
    } else {
        pred->next = curr->next;
    }

    free(curr);
    pthread_rwlock_unlock(&list_lock);
    return 1;
}

void free_list(void) {
    /* TODO: free every node, reset head to NULL, destroy list_lock */
    node_t *curr = head;
    while (curr != NULL) {
        node_t *next = curr->next;
        free(curr);
        curr = next;
    }
    head = NULL;
    pthread_rwlock_destroy(&list_lock);
}