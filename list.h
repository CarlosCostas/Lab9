#ifndef LIST_H
#define LIST_H

/* Sorted singly linked list of integers, strictly ascending, no duplicates.
 * Thread-safe: concurrent calls from multiple threads are allowed.
 */

/* Initialize the global list and its rwlock. Call once before other calls. */
void list_init(void);

/* Returns 1 if value is in the list, 0 otherwise. Uses a read lock. */
int Member(int value);

/* Inserts value in sorted position. Returns 1 if inserted, 0 if already present.
 * Uses a write lock.
 */
int Insert(int value);

/* Deletes value if present. Returns 1 if deleted, 0 if not found.
 * Uses a write lock.
 */
int Delete(int value);

/* Frees all nodes and destroys the rwlock. Call after all threads have finished. */
void free_list(void);

#endif
