#ifndef BASE64_H
#define BASE64_H
#include <stddef.h>

size_t b64_enc_size(size_t inlen);
size_t b64_deco_size(const char *in);

char *b64_encode(const unsigned char *in, size_t len);
int b64_decode(const char *in, unsigned char *out, size_t outlen);





#endif