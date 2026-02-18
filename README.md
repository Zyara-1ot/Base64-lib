# Base64 Library in C

A lightweight Base64 encoder and decoder implemented from scratch in C.

## Features

- Base64 encoding
- Base64 decoding
- Proper padding handling (`=`)
- Dynamic memory allocation
- Clean and modular implementation

  ## What It Does

This project provides a minimal implementation of the Base64 algorithm
as defined in RFC 4648.

## Usage

### Include in Your Project

1. Copy `base64.h` and `base64.c` to your project
2. Include the header in your code:
```c
#include "base64.h"
```

### Encoding Example
```c
#include <stdio.h>
#include <string.h>
#include "base64.h"

int main() {
    const char *data = "Hello, World!";
    
    // Encode
    char *encoded = b64_encode((unsigned char *)data, strlen(data));
    printf("Encoded: %s\n", encoded);
    
    free(encoded);
    return 0;
}
```

### Decoding Example
```c
#include <stdio.h>
#include <stdlib.h>
#include "base64.h"

int main() {
    const char *base64_string = "SGVsbG8sIFdvcmxkIQ==";
    
    // Calculate decoded size
    size_t decoded_size = b64_decoded_size(base64_string);
    
    // Allocate buffer
    unsigned char *decoded = malloc(decoded_size + 1);
    
    // Decode
    if (b64_decode(base64_string, decoded, decoded_size)) {
        decoded[decoded_size] = '\0';
        printf("Decoded: %s\n", decoded);
    }
    
    free(decoded);
    return 0;
}
```

### Compilation
```bash
gcc -o myprogram myprogram.c base64.c
```

### Encoding
- Processes input in 3-byte (24-bit) blocks
- Extracts four 6-bit segments
- Maps each segment to the Base64 character table
- Applies `=` padding when required

### Decoding
- Converts Base64 characters back into 6-bit values
- Reconstructs the original 24-bit block
- Extracts the original 8-bit bytes
- Validates input and handles padding safely


## Project Structure

base64.c   - Core encoding and decoding logic  
base64.h   - Header declarations  
main.c     - Example usage and test driver  

## Build

```bash
gcc -Wall -Wextra -o base64 main.c base64.c

## Run

./base64


## Example Output

data:    'ABC123Test Lets Try this' input and see What "happens"'
encoded: '...'
dec:     'ABC123Test Lets Try this' input and see What "happens"'

