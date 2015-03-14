## lioness - LIONESS wide block cipher
### Yawning Angel (yawning at torproject dot org)

This is a simple LIONESS implementation based on the ChaCha stream cipher and
the BLAKE2 hash algorithm.  It was written primarily to get a feel for just how
bad LIONESS performance is with modern cryptographic primitives.

Primitive implementations (./ext/):
 * The BLAKE2 reference implementation (https://github.com/BLAKE2/BLAKE2)
 * Ted Krovet's "vec128" ChaCha implementation from SUPERCOP.

Notes:
 * DO NOT USE THIS IN PRODUCTION CODE.
 * The original paper is at http://www.cl.cam.ac.uk/~rja14/Papers/bear-lion.pdf
 * Yes, this uses `alloca()`, fite me IRL.
