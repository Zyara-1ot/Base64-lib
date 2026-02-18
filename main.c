#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"

int main(void)
{
    const char *data = "ABC123Test Lets Try this' input and see What \"happens\"";
    char       *enc;
    char       *out;
    size_t      out_len;
    
    printf("data:    '%s'\n", data);
    
    enc = b64_encode((const unsigned char *)data, strlen(data));
    printf("encoded: '%s'\n", enc);
    
    printf("dec size %s data size\n", b64_deco_size(enc) == strlen(data) ? "==" : "!=");
    
    out_len = b64_deco_size(enc);
    out = malloc(out_len + 1); 
    
    if (!b64_decode(enc, (unsigned char *)out, out_len)) {
        printf("Decode Failure\n");
        free(enc); 
        free(out);
        return 1;
    }
    
    out[out_len] = '\0'; 
    
    printf("dec:     '%s'\n", out);
    printf("data %s dec\n", strcmp(data, out) == 0 ? "==" : "!=");
    
    free(enc);  
    free(out);  
    
    return 0;
}