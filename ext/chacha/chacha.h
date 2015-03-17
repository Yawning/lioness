/*
 * To the extent possible under law, the Yawning Angel has waived all copyright
 * and related or neighboring rights to lioness, using the creative
 * commons "cc0" public domain dedication. See LICENSE or
 * <http://creativecommons.org/publicdomain/zero/1.0/> for full details.
 */

#ifndef CHACHA_H_INCLUDED_
#define CHACHA_H_INCLUDED_

#define CHACHA_KEY_LEN (256/8)
#define CHACHA_NONCE_LEN 8

int chacha_stream_xor(
        unsigned char *out,
        const unsigned char *in,
        unsigned long long inlen,
        const unsigned char *n,
        const unsigned char *k);

#endif
