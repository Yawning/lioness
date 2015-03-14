/*
 * To the extent possible under law, the Yawning Angel has waived all copyright
 * and related or neighboring rights to or-ctl-filter, using the creative
 * commons "cc0" public domain dedication. See LICENSE or
 * <http://creativecommons.org/publicdomain/zero/1.0/> for full details.
 */

#ifndef CHACHA_H_INCLUDED_
#define CHACHA_H_INCLUDED_

int chacha_crypto_stream(unsigned char *out,
                         unsigned long long outlen,
                         const unsigned char *n,
                         const unsigned char *k);

#endif
