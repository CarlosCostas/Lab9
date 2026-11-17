CC      = gcc
CFLAGS  = -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread -g -O2
LDFLAGS = -pthread

all: test_local stress_sample bench

test_local: test_local.c list.c list.h
	$(CC) $(CFLAGS) -o $@ test_local.c list.c

stress_sample: stress_sample.c list.c barrier.c list.h barrier.h
	$(CC) $(CFLAGS) -o $@ stress_sample.c list.c barrier.c

bench: bench.c list.c list.h
	$(CC) $(CFLAGS) -o $@ bench.c list.c

valgrind: test_local stress_sample
	valgrind --leak-check=full --error-exitcode=1 ./test_local
	valgrind --leak-check=full --error-exitcode=1 ./stress_sample

tsan: stress_sample.c list.c barrier.c list.h barrier.h
	$(CC) -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread -g -O1 \
	    -fsanitize=thread -o stress_tsan stress_sample.c list.c barrier.c
	./stress_tsan

clean:
	rm -f test_local stress_sample bench stress_tsan

.PHONY: all valgrind tsan clean
