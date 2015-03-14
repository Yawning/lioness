/* To the extent possible under law, the Yawning Angel has waived all copyright
 * and related or neighboring rights to or-ctl-filter, using the creative
 * commons "cc0" public domain dedication. See LICENSE or
 * <http://creativecommons.org/publicdomain/zero/1.0/> for full details.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lioness.h"

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  /* key = [0 ... 207] */
  uint8_t key[LIONESS_KEY_LEN];
  for (size_t i = 0; i < sizeof(key); i++) {
    key[i] = i & 0xff;
  }

  const size_t block_sz = 509;
  lioness_t *l = lioness_new(key, sizeof(key), block_sz);
  if (l == NULL) {
    fprintf(stderr, "lioness_new() failed\n");
    return -1;
  }

  const char *vec = "'What do we know,' he had said, 'of the world and the universe about us? Our means of receiving impressions are absurdly few, and our notions of surrounding objects infinitely narrow. We see things only as we are constructed to see them, and can gain no idea of their absolute nature. With five feeble senses we pretend to comprehend the boundlessly complex cosmos, yet other beings with wider, stronger, or different range of senses might not only see very differently the things we see, but might see and st";

  /* Encrypt and see if it blows up. */
  uint8_t block[block_sz];
  lioness_encrypt_block(l, block, (const uint8_t *)vec);
  if (0 == memcmp(vec, block, block_sz)) {
    fprintf(stderr, "lioness_encrypt_block() did nothing?\n");
    return -1;
  }

  /* Decrypt and see if it blows up. */
  lioness_decrypt_block(l, block, block);
  if (0 != memcmp(vec, block, block_sz)) {
    fprintf(stderr, "lioness_decrypt_block() failed?\n");
    return -1;
  }

  /* Quick and dirty benchmark that shouldn't be taken seriously. */
  struct timespec start;
  struct timespec end;
  const int max_iters = 1000000;

  clock_gettime(CLOCK_MONOTONIC, &start);
  for (int i = 0; i < max_iters; i++) {
    lioness_encrypt_block(l, block, block);
  }
  clock_gettime(CLOCK_MONOTONIC, &end);

  uint64_t elapsed_ns = 0;
	if ((end.tv_nsec - start.tv_nsec) < 0) {
		elapsed_ns = (end.tv_sec - start.tv_sec-1) * 1000000000;
		elapsed_ns += 1000000000 + end.tv_nsec-start.tv_nsec;
	} else {
		elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000;
		elapsed_ns += end.tv_nsec - start.tv_nsec;
	}
  double secs = (double)elapsed_ns / 1000000000;
  fprintf(stdout, "Elapsed: %f s (%f Bytes/s, %f ns/byte)\n",
          secs,
          block_sz * max_iters / secs,
          (double)elapsed_ns/ (block_sz * max_iters));

  lioness_free(l);

  return 0;
}
