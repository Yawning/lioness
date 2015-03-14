/* To the extent possible under law, the Yawning Angel has waived all copyright
 * and related or neighboring rights to or-ctl-filter, using the creative
 * commons "cc0" public domain dedication. See LICENSE or
 * <http://creativecommons.org/publicdomain/zero/1.0/> for full details.
 */

#include <stdlib.h>
#include <string.h>
#include <alloca.h>
#include <assert.h>

#include "lioness.h"
#include "ext/chacha/chacha.h"
#include "ext/blake2/sse/blake2.h"

static void * (*volatile memset_volatile)(void *, int, size_t) = memset;
static inline void xorbytes(uint8_t *d, const uint8_t *a, const uint8_t *b, size_t len);

#define S_KEY_LEN (CHACHA_KEY_LEN + CHACHA_NONCE_LEN)
#define H_KEY_LEN BLAKE2B_KEYBYTES
#define MIN_BLOCK_SZ S_KEY_LEN

struct lioness_s {
  uint8_t k1[S_KEY_LEN];
  uint8_t k2[H_KEY_LEN];
  uint8_t k3[S_KEY_LEN];
  uint8_t k4[H_KEY_LEN];
  size_t block_sz;
};

lioness_t *
lioness_new(const uint8_t *key, size_t key_len, size_t block_sz)
{
  lioness_t *s = NULL;

  /* The block size must accomodate |L| = S_KEY_LEN, along with 
   * |R| > 0, and the key should be the correct size.
   */
  if (block_sz <= MIN_BLOCK_SZ || key_len != LIONESS_KEY_LEN)
    return s;

  s = calloc(1, sizeof(lioness_t));
  memcpy(s->k1, key, S_KEY_LEN);
  memcpy(s->k2, key + S_KEY_LEN, H_KEY_LEN);
  memcpy(s->k3, key + (S_KEY_LEN + H_KEY_LEN), S_KEY_LEN);
  memcpy(s->k4, key + (2 * S_KEY_LEN + H_KEY_LEN), H_KEY_LEN);
  s->block_sz = block_sz;

  return s;
}

void
lioness_free(lioness_t *s)
{
  memset_volatile(s, 0, sizeof(*s));
  free(s);
}

void
lioness_encrypt_block(const lioness_t *s, uint8_t *out, const uint8_t *in)
{
  const size_t l_sz = S_KEY_LEN;
  const size_t r_sz = s->block_sz - l_sz;
  uint8_t tmp[l_sz];
  uint8_t l[l_sz];
  uint8_t *r = alloca(r_sz);

  /* R = R ^ S(L ^ K1) */
  xorbytes(tmp, in, s->k1, l_sz);
  chacha_stream_xor(r, in + l_sz, r_sz, tmp + CHACHA_NONCE_LEN, tmp);
  /* L = L ^ H(K2, R) */
  blake2b(tmp, r, s->k2, l_sz, r_sz, H_KEY_LEN);
  xorbytes(l, in, tmp, l_sz);
  /* R = R ^ S(L ^ K3) */
  xorbytes(tmp, l, s->k3, S_KEY_LEN);
  chacha_stream_xor(r, r, r_sz, tmp + CHACHA_NONCE_LEN, tmp);
  /* L = L ^ H(K4, R) */
  blake2b(tmp, r, s->k4, l_sz, r_sz, H_KEY_LEN);
  xorbytes(l, l, tmp, l_sz);

  memcpy(out, l, l_sz);
  memcpy(out + l_sz, r, r_sz);
  memset_volatile(tmp, 0, sizeof(tmp));
}

void
lioness_decrypt_block(const lioness_t *s, uint8_t *out, const uint8_t *in)
{
  const size_t l_sz = S_KEY_LEN;
  const size_t r_sz = s->block_sz - l_sz;
  uint8_t tmp[l_sz];
  uint8_t l[l_sz];
  uint8_t *r = alloca(r_sz);

  /* L = L ^ H(K4, R) */
  blake2b(tmp, in + l_sz, s->k4, l_sz, r_sz, H_KEY_LEN);
  xorbytes(l, in, tmp, l_sz);
  /* R = R ^ S(L ^ K3) */
  xorbytes(tmp, l, s->k3, S_KEY_LEN);
#ifdef __SSSE3__
  chacha_stream_xor(r, in + l_sz, r_sz, tmp + CHACHA_NONCE_LEN, tmp);
#else
  /* Force alignment to be right for the current ChaCha impl. */
  memcpy(r, in + l_sz, r_sz);
  chacha_stream_xor(r, r, r_sz, tmp + CHACHA_NONCE_LEN, tmp);
#endif
  /* L = L ^ H(K2, R) */
  blake2b(tmp, r, s->k2, l_sz, r_sz, H_KEY_LEN);
  xorbytes(l, l, tmp, l_sz);
  /* R = R ^ S(L ^ K1) */
  xorbytes(tmp, l, s->k1, S_KEY_LEN);
  chacha_stream_xor(r, r, r_sz, tmp + CHACHA_NONCE_LEN, tmp);

  memcpy(out, l, l_sz);
  memcpy(out + l_sz, r, r_sz);
  memset_volatile(tmp, 0, sizeof(tmp));
}

static inline void
xorbytes(uint8_t *d, const uint8_t *a, const uint8_t *b, size_t len) {
  size_t i = 0;
  for (i = 0; i < len; i++) {
    d[i] = a[i] ^ b[i];
  }
}
