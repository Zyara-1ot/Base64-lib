// 12 to 16
#include "base64.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


static const char b64chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int b64invs[] = {
    62,-1,-1,-1,63,52,53,54,55,56,57,58,
    59,60,61,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,
    6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
    21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,
    43,44,45,46,47,48,49,50,51
};


size_t b64_enc_size(size_t inlen){
    size_t res = inlen;
    if (inlen % 3 != 0)
        res += 3 - (inlen % 3);
    res = (res / 3) * 4;
    return res;
}

char *b64_encode(const unsigned char *in, size_t len){
    if (!in || len == 0) return NULL;

    size_t elen = b64_enc_size(len);
    char *out = malloc(elen + 1);
    if (!out) return NULL;
    out[elen] = '\0';

    size_t i = 0, j = 0;
    uint32_t v;
    while (i + 12 <= len) {

        v = ((uint32_t)in[i] << 16) |
            ((uint32_t)in[i+1] << 8) |
            in[i+2];

        out[j]   = b64chars[(v >> 18) & 0x3F];
        out[j+1] = b64chars[(v >> 12) & 0x3F];
        out[j+2] = b64chars[(v >> 6) & 0x3F];
        out[j+3] = b64chars[v & 0x3F];

        v = ((uint32_t)in[i+3] << 16) |
            ((uint32_t)in[i+4] << 8) |
            in[i+5];

        out[j+4] = b64chars[(v >> 18) & 0x3F];
        out[j+5] = b64chars[(v >> 12) & 0x3F];
        out[j+6] = b64chars[(v >> 6) & 0x3F];
        out[j+7] = b64chars[v & 0x3F];
        v = ((uint32_t)in[i+6] << 16) |((uint32_t)in[i+7] << 8) | in[i+8];
        out[j+8]  = b64chars[(v >> 18) & 0x3F];
        out[j+9]  = b64chars[(v >> 12) & 0x3F];
        out[j+10] = b64chars[(v >> 6) & 0x3F];
        out[j+11] = b64chars[v & 0x3F];
        v = ((uint32_t)in[i+9] << 16) |((uint32_t)in[i+10] << 8) | in[i+11];
        out[j+12] = b64chars[(v >> 18) & 0x3F];
        out[j+13] = b64chars[(v >> 12) & 0x3F];
        out[j+14] = b64chars[(v >> 6) & 0x3F];
        out[j+15] = b64chars[v & 0x3F];

        i += 12;
        j += 16;
    }
    while (i + 3 <= len) {
        v = ((uint32_t)in[i] << 16) |
            ((uint32_t)in[i+1] << 8) |
            in[i+2];

        out[j]   = b64chars[(v >> 18) & 0x3F];
        out[j+1] = b64chars[(v >> 12) & 0x3F];
        out[j+2] = b64chars[(v >> 6) & 0x3F];
        out[j+3] = b64chars[v & 0x3F];

        i += 3;
        j += 4;
    }
    if (i < len) {
        v = (uint32_t)in[i] << 16;
        if (i+1 < len) v |= (uint32_t)in[i+1] << 8;
        if (i+2 < len) v |= (uint32_t)in[i+2];

        out[j]   = b64chars[(v >> 18) & 0x3F];
        out[j+1] = b64chars[(v >> 12) & 0x3F];
        out[j+2] = (i+1 < len) ? b64chars[(v >> 6) & 0x3F] : '=';
        out[j+3] = (i+2 < len) ? b64chars[v & 0x3F] : '=';
    }

    return out;
}

size_t b64_deco_size(const char *in){
    if (!in) return 0;
    size_t len = strlen(in);
    size_t ret = (len / 4) * 3;
    for (size_t i = len; i-- > 0;) {
        if (in[i] == '=')
            ret--;
        else
            break;
    }
    return ret;
}

int b64_isvalidchar(char c){
    if (c >= '0' && c <= '9') return 1;
    if (c >= 'A' && c <= 'Z') return 1;
    if (c >= 'a' && c <= 'z') return 1;
    if (c == '+' || c == '/' || c == '=') return 1;
    return 0;
}

int b64_decode(const char *in, unsigned char *out, size_t outlen){
    if (!in || !out) return 0;

    size_t len = strlen(in);
    if (outlen < b64_deco_size(in) || len % 4 != 0)
        return 0;
    for (size_t i = 0; i < len; i++)
        if (!b64_isvalidchar(in[i]))
            return 0;
    size_t i = 0, j = 0;
    int v1, v2;

    while (i + 8 <= len && in[i+7] != '=') {
        v1 = b64invs[in[i] - 43];
        v1 = (v1 << 6) | b64invs[in[i+1] - 43];
        v1 = (v1 << 6) | b64invs[in[i+2] - 43];
        v1 = (v1 << 6) | b64invs[in[i+3] - 43];

        v2 = b64invs[in[i+4] - 43];
        v2 = (v2 << 6) | b64invs[in[i+5] - 43];
        v2 = (v2 << 6) | b64invs[in[i+6] - 43];
        v2 = (v2 << 6) | b64invs[in[i+7] - 43];

        out[j]   = (v1 >> 16) & 0xFF;
        out[j+1] = (v1 >> 8) & 0xFF;
        out[j+2] = v1 & 0xFF;
        out[j+3] = (v2 >> 16) & 0xFF;
        out[j+4] = (v2 >> 8) & 0xFF;
        out[j+5] = v2 & 0xFF;
        i += 8;
        j += 6;
    }

    while (i < len) {
        v1 = b64invs[in[i] - 43];
        v1 = (v1 << 6) | b64invs[in[i+1] - 43];
        v1 = in[i+2]=='=' ? v1 << 6 : (v1 << 6) | b64invs[in[i+2]-43];
        v1 = in[i+3]=='=' ? v1 << 6 : (v1 << 6) | b64invs[in[i+3]-43];

        out[j] = (v1 >> 16) & 0xFF;
        if (in[i+2] != '=')
            out[j+1] = (v1 >> 8) & 0xFF;
        if (in[i+3] != '=')
            out[j+2] = v1 & 0xFF;

        i += 4;
        j += 3;
    }

    return 1;
}
