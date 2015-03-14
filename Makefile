CC := gcc
CFLAGS := -std=gnu99 -Wall -Wextra -Werror -fno-strict-aliasing
OPTFLAGS := -march=native -O3

all: clean lioness_test

clean:
	rm -f lioness_test

lioness_test:
	$(CC) lioness_test.c lioness.c ext/chacha_krovetz.c ext/blake2/sse/blake2b.c -o $@ $(CFLAGS) $(OPTFLAGS)
