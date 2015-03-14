/*
 * To the extent possible under law, the Yawning Angel has waived all copyright
 * and related or neighboring rights to or-ctl-filter, using the creative
 * commons "cc0" public domain dedication. See LICENSE or
 * <http://creativecommons.org/publicdomain/zero/1.0/> for full details.
 */

#include <stddef.h>
#include <stdint.h>

#ifndef LIONESS_H_INCLUDED_
#define LIONESS_H_INCLUDED_

#define LIONESS_KEY_LEN 208

struct lioness_s;
typedef struct lioness_s lioness_t;

lioness_t *lioness_new(const uint8_t *key, size_t key_len, size_t block_sz);
void lioness_free(lioness_t *s);

void lioness_encrypt_block(const lioness_t *s, uint8_t *out, const uint8_t *in);
void lioness_decrypt_block(const lioness_t *s, uint8_t *out, const uint8_t *in);

#endif
