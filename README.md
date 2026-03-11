# Base64 Encoder/Decoder in C

A high-performance Base64 library written from scratch in pure C, achieving competitive speeds with industry-standard implementations through algorithmic optimization.

## Performance

Benchmarked on AMD Ryzen 5 5500U (6 cores, 12 threads), compiled with GCC and `-O3 -march=native`:

| Implementation | Throughput | Relative Performance |
|----------------|------------|---------------------|
| GNU coreutils  | 340 MB/s   | Reference           |
| OpenSSL        | 302 MB/s   | -11%                |
| **This library** | **271 MB/s** | **-20%**        |
| libb64 (2010)  | ~32 MB/s*  | -91%                |

*libb64 result from their 2010 benchmark on different hardware

### Key Achievement
Achieved **271 MB/s throughput** using only standard C optimizations—no SIMD, no assembly, just smart algorithm design.

## Features

-  Proper padding handling (`=`)
-  High-performance loop unrolling (12→16 byte batching)
-  Branch-free main loops for better CPU pipelining
-  Clean, readable, well-documented code
-  Zero external dependencies

## Installation

```bash
# Clone the repository
git clone https://github.com/Zyara-1ot/Base64-lib.git
cd base64
```     

## Usage

### Basic Example

```c
#include "base64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Encoding
    const char *data = "Hello, World!";
    char *encoded = b64_encode((unsigned char *)data, strlen(data));
    
    if (encoded) {
        printf("Encoded: %s\n", encoded);
        
        // Decoding
        size_t decoded_size = b64_decoded_size(encoded);
        unsigned char *decoded = malloc(decoded_size + 1);
        
        if (b64_decode(encoded, decoded, decoded_size)) {
            decoded[decoded_size] = '\0';
            printf("Decoded: %s\n", decoded);
        }
        
        free(decoded);
        free(encoded);
    }
    
    return 0;
}
```

### Encoding

```c
char *b64_encode(const unsigned char *in, size_t len);
```

**Parameters:**
- `in` - Input data to encode
- `len` - Length of input data in bytes

**Returns:**
- Pointer to newly allocated Base64 string (caller must `free()`)
- `NULL` on error

**Example:**
```c
unsigned char data[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f}; // "Hello"
char *encoded = b64_encode(data, 5);
// Result: "SGVsbG8="
free(encoded);
```

### Decoding

```c
int b64_decode(const char *in, unsigned char *out, size_t outlen);
```

**Parameters:**
- `in` - Base64 string to decode
- `out` - Pre-allocated output buffer
- `outlen` - Size of output buffer

**Returns:**
- `1` on success
- `0` on error (invalid input, buffer too small)

**Example:**
```c
const char *encoded = "SGVsbG8=";
size_t size = b64_decoded_size(encoded);
unsigned char *decoded = malloc(size);

if (b64_decode(encoded, decoded, size)) {
    // Success! decoded contains the data
}
free(decoded);
```

### Helper Functions

```c
size_t b64_enc_size(size_t inlen);
```
Calculate required output buffer size for encoding.

```c
size_t b64_decoded_size(const char *in);
```
Calculate required output buffer size for decoding.

## API Reference

### Functions

| Function | Description |
|----------|-------------|
| `b64_encode()` | Encode binary data to Base64 string |
| `b64_decode()` | Decode Base64 string to binary data |
| `b64_enc_size()` | Calculate encoded output size |
| `b64_decoded_size()` | Calculate decoded output size |
| `b64_isvalidchar()` | Validate Base64 character |

### Memory Management

- **Encoding:** Returns `malloc`'d memory—caller must `free()`
- **Decoding:** Writes to caller-provided buffer—caller manages allocation
- Always allocate `size + 1` for null terminator when treating output as string

## Implementation Details

### Optimization Techniques

1. **Loop Unrolling (12→16)**
   - Processes 12 input bytes → 16 Base64 characters per iteration
   - Reduces loop overhead by 4x compared to naive 3→4 approach
   - Enables better CPU instruction pipelining

2. **Branch-Free Main Loop**
   - Separates padding logic from main encoding loop
   - Eliminates branch mispredictions in hot path
   - Improves performance by ~15-20%

3. **Batch Decoding (8→6)**
   - Processes 8 Base64 characters → 6 output bytes per iteration
   - Reduces decode loop iterations by 2x

### Why No SIMD?

This implementation prioritizes:
- **Portability** - Works on any C99-compliant compiler
- **Readability** - Code is straightforward and maintainable
- **Learning** - Demonstrates algorithmic optimization techniques

SIMD could provide 2-4x additional speedup but at the cost of:
- Platform-specific code (x86 AVX2, ARM NEON)
- Increased complexity (200+ lines → 1000+ lines)
- Harder to maintain and understand

Current performance (271 MB/s) is sufficient for most use cases where disk I/O is the bottleneck.

## Benchmarking

### Run Benchmarks

```bash
# Compile benchmark
gcc -O3 -march=native -o file_benchmark file_benchmark.c base64c.c

# Make benchmark script executable
chmod +x benchmark_comparison.sh

# For accurate results, run in tmpfs (eliminates disk I/O bottleneck)
cp file_benchmark benchmark_comparison.sh /dev/shm/
cd /dev/shm
./benchmark_comparison.sh

# Or run directly (includes disk I/O in measurement)
./benchmark_comparison.sh
```

**Note:** Running in `/dev/shm` (RAM disk) eliminates disk I/O as a bottleneck, providing more accurate CPU/algorithm performance measurements. However, reported speeds in this README include realistic file I/O overhead.

### Benchmark Methodology

- **Test data:** 18 MB random binary data
- **Iterations:** 50 encode + decode round-trips
- **Runs:** 5 runs averaged (with warm-up)
- **Compiler flags:** `-O3 -march=native -flto`
- **Measurement:** Total data processed / elapsed time

### Performance Tips

**For maximum accuracy:**
1. Run benchmarks in `/dev/shm` (tmpfs/RAM disk) to eliminate disk I/O
2. Close other applications to reduce system noise
3. Disable CPU frequency scaling: `sudo cpupower frequency-set -g performance`
4. Run multiple iterations and average results

**Reported speeds (271 MB/s) include realistic file I/O overhead from SSD.**

### Why These Flags?

Compiler optimizations (`-O3`, `-march=native`) are standard practice in performance benchmarking:
- Industry tools (GNU, OpenSSL) are compiled with optimizations
- Measures algorithmic performance, not compiler limitations
- 
## Project Structure

```
base64/
├── base64.h              # API declarations
├── base64.c              # Core implementation
├── main.c                # Example usage
├── file_benchmark.c      # File I/O benchmark tool
├── benchmark_comparison.sh  # Multi-implementation benchmark
└── README.md             
```

## Building

### Basic Build

```bash
gcc -O3 -march=native -o test main.c base64.c
./test
```


### Compilation Flags

| Flag | Purpose |
|------|---------|
| `-O3` | Maximum optimization |
| `-march=native` | Use CPU-specific instructions |
| `-flto` | Link-time optimization |
| `-funroll-loops` | Additional loop unrolling |

## Testing

```bash
# Basic correctness test
./test

# Expected output:
# data:    'Test string here'
# encoded: 'VGVzdCBzdHJpbmcgaGVyZQ=='
# dec size == data size
# dec:     'Test string here'
# data == dec
```

## Limitations

- Maximum input size: Limited by available memory
- No streaming API (processes entire input at once)
- Decode assumes valid Base64 input (validation included)
- Not constant-time (not suitable for cryptographic contexts where timing attacks are a concern)


## Author

Built from scratch as a learning project to understand Base64 encoding at the bit level and explore performance optimization techniques.

---

**Performance, simplicity, and clarity—all in pure C.** 
