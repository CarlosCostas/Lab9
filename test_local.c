#include <stdio.h>
#include <stdlib.h>
#include "list.h"

static int failures = 0;

#define CHECK(cond, msg)                                  \
    do {                                                  \
        if (!(cond)) {                                    \
            fprintf(stderr, "FAIL: %s (line %d)\n",       \
                    msg, __LINE__);                       \
            failures++;                                   \
        } else {                                          \
            printf("PASS: %s\n", msg);                    \
        }                                                 \
    } while (0)

int main(void) {
    list_init();

    /* Empty list */
    CHECK(Member(5) == 0, "Member on empty list returns 0");
    CHECK(Delete(5) == 0, "Delete on empty list returns 0");

    /* Basic inserts */
    CHECK(Insert(10) == 1, "Insert 10 succeeds");
    CHECK(Insert(5) == 1,  "Insert 5 (new head) succeeds");
    CHECK(Insert(20) == 1, "Insert 20 (new tail) succeeds");
    CHECK(Insert(10) == 0, "Insert duplicate 10 returns 0");

    /* Membership */
    CHECK(Member(5) == 1,  "Member finds head value 5");
    CHECK(Member(10) == 1, "Member finds middle value 10");
    CHECK(Member(20) == 1, "Member finds tail value 20");
    CHECK(Member(7) == 0,  "Member does not find absent value 7");

    /* Deletes */
    CHECK(Delete(5) == 1,  "Delete head 5 succeeds");
    CHECK(Member(5) == 0,  "5 is gone after delete");
    CHECK(Delete(20) == 1, "Delete tail 20 succeeds");
    CHECK(Member(20) == 0, "20 is gone after delete");
    CHECK(Delete(10) == 1, "Delete last remaining 10 succeeds");
    CHECK(Delete(10) == 0, "Delete already-deleted 10 returns 0");

    /* Ordering: insert out of order, verify all present */
    int vals[] = {42, 7, 99, 1, 50};
    for (int i = 0; i < 5; i++) {
        CHECK(Insert(vals[i]) == 1, "Insert out-of-order value");
    }
    for (int i = 0; i < 5; i++) {
        CHECK(Member(vals[i]) == 1, "Member finds out-of-order value");
    }

    free_list();

    if (failures) {
        fprintf(stderr, "%d check(s) failed\n", failures);
        return EXIT_FAILURE;
    }
    printf("All local tests passed\n");
    return EXIT_SUCCESS;
}
