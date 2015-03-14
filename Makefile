CC := gcc
CFLAGS := -std=gnu99 -Wall -Wextra -Werror -fno-strict-aliasing
OPTFLAGS := -march=native -O3

all: clean lioness_test_vec128 lioness_test_avx2

clean:
	rm -f lioness_test_vec128 lioness_test_avx2

lioness_test_vec128:
	$(CC) lioness_test.c lioness.c ext/chacha/chacha_krovetz_vec128.c ext/blake2/sse/blake2b.c -o $@ $(CFLAGS) $(OPTFLAGS)

lioness_test_avx2:
	$(CC) lioness_test.c lioness.c ext/chacha/chacha_krovetz_avx2.c ext/blake2/sse/blake2b.c -o $@ $(CFLAGS) $(OPTFLAGS)
