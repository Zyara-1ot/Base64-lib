#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "base64.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <encode|decode> <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    char *mode = argv[1];
    char *input_file = argv[2];
    char *output_file = argv[3];
    
    clock_t start = clock();
    
    if (strcmp(mode, "encode") == 0) {
        FILE *in = fopen(input_file, "rb");
        if (!in) {
            perror("Failed to open input file");
            return 1;
        }
        fseek(in, 0, SEEK_END);
        size_t size = ftell(in);
        fseek(in, 0, SEEK_SET);
        
        unsigned char *data = malloc(size);
        fread(data, 1, size, in);
        fclose(in);
        
        char *encoded = b64_encode(data, size);
        FILE *out = fopen(output_file, "w");
        fprintf(out, "%s", encoded);
        fclose(out);
        
        free(data);
        free(encoded);
        
    } else if (strcmp(mode, "decode") == 0) {
        FILE *in = fopen(input_file, "r");
        if (!in) {
            perror("Failed to open input file");
            return 1;
        }
        
        fseek(in, 0, SEEK_END);
        size_t size = ftell(in);
        fseek(in, 0, SEEK_SET);
        
        char *encoded = malloc(size + 1);
        fread(encoded, 1, size, in);
        encoded[size] = '\0';
        fclose(in);
        
        size_t deco_size = b64_deco_size(encoded);
        unsigned char *decoded = malloc(deco_size);
        
        if (!b64_decode(encoded, decoded, deco_size)) {
            fprintf(stderr, "Decode failed\n");
            free(encoded);
            free(decoded);
            return 1;
        }
        
        FILE *out = fopen(output_file, "wb");
        fwrite(decoded, 1, deco_size, out);
        fclose(out);
        
        free(encoded);
        free(decoded);
        
    } else {
        printf("Invalid mode. Use 'encode' or 'decode'\n");
        return 1;
    }
    
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Time: %.3f seconds\n", elapsed);
    
    return 0;
}