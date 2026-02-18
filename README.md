# Base64 Library in C

A lightweight Base64 encoder and decoder implemented from scratch in C.

## Features

- Base64 encoding
- Base64 decoding
- Proper padding handling (`=`)
- Dynamic memory allocation
- Clean and modular implementation

## Project Structure

base64.c   - Core encoding and decoding logic  
base64.h   - Header declarations  
main.c     - Example usage and test driver  

## 🔧 Build

```bash
gcc -Wall -Wextra -o base64 main.c base64.c

## Run

./base64


## Example Output

data:    'ABC123Test Lets Try this' input and see What "happens"'
encoded: '...'
dec:     'ABC123Test Lets Try this' input and see What "happens"'

