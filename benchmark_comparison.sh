#!/bin/bash

FILE_SIZE_MB=18
ITERATIONS=50
RUNS=5
TOTAL_MB=$((FILE_SIZE_MB * ITERATIONS * 2))

CPU_MODEL=$(lscpu | grep "Model name" | sed 's/Model name:[ ]*//')
CORES=$(lscpu | grep "^Core(s) per socket" | awk '{print $4}')
THREADS_PER_CORE=$(lscpu | grep "Thread(s) per core" | awk '{print $4}')
TOTAL_THREADS=$(nproc)
OS_INFO=$(uname -srmo)

echo "CPU: $CPU_MODEL"
echo "Cores per socket: $CORES"
echo "Threads per core: $THREADS_PER_CORE"
echo "Total threads: $TOTAL_THREADS"
echo "OS: $OS_INFO"
echo "Run type: Hot run"
echo ""

echo "File size: ${FILE_SIZE_MB} MB"
echo "Iterations per run: ${ITERATIONS}"
echo "Runs: ${RUNS}"
echo "Total processed per run: ${TOTAL_MB} MB"

echo "Creating test file..."
dd if=/dev/zero of=test_input.bin bs=1M count=$FILE_SIZE_MB 2>/dev/null
echo ""

benchmark() {
    name=$1
    mode=$2

    echo "Testing: $name"

    echo "Warm-up run (not measured)..."

    # Warm-up
    for i in $(seq 1 $ITERATIONS); do
        if [ "$mode" -eq 1 ]; then
            ./file_benchmark encode test_input.bin test_encoded.b64 > /dev/null 2>&1
            ./file_benchmark decode test_encoded.b64 test_decoded.bin > /dev/null 2>&1
        elif [ "$mode" -eq 2 ]; then
            base64 test_input.bin > test_encoded.b64 2>/dev/null
            base64 -d test_encoded.b64 > test_decoded.bin 2>/dev/null
        elif [ "$mode" -eq 3 ]; then
            openssl base64 -in test_input.bin -out test_encoded.b64 2>/dev/null
            openssl base64 -d -in test_encoded.b64 -out test_decoded.bin 2>/dev/null
        fi
    done

    echo "Warm-up complete."
    echo ""

    sum=0
    min=999999
    max=0
    for run in $(seq 1 $RUNS); do
        start=$(date +%s.%N)
        for i in $(seq 1 $ITERATIONS); do
            if [ "$mode" -eq 1 ]; then
                ./file_benchmark encode test_input.bin test_encoded.b64 > /dev/null 2>&1
                ./file_benchmark decode test_encoded.b64 test_decoded.bin > /dev/null 2>&1
            elif [ "$mode" -eq 2 ]; then
                base64 test_input.bin > test_encoded.b64 2>/dev/null
                base64 -d test_encoded.b64 > test_decoded.bin 2>/dev/null
            elif [ "$mode" -eq 3 ]; then
                openssl base64 -in test_input.bin -out test_encoded.b64 2>/dev/null
                openssl base64 -d -in test_encoded.b64 -out test_decoded.bin 2>/dev/null
            fi
        done

        end=$(date +%s.%N)
        elapsed=$(echo "$end - $start" | bc -l)
        speed=$(echo "$TOTAL_MB / $elapsed" | bc -l)
        printf "Run %d: %.2f MB/s\n" "$run" "$speed"
        sum=$(echo "$sum + $speed" | bc -l)
        if (( $(echo "$speed < $min" | bc -l) )); then
            min=$speed
        fi
        if (( $(echo "$speed > $max" | bc -l) )); then
            max=$speed
        fi
    done
    avg=$(echo "$sum / $RUNS" | bc -l)

    echo ""
    printf "Average (excluding warm-up): %.2f MB/s\n" "$avg"
    printf "Min: %.2f MB/s\n" "$min"
    printf "Max: %.2f MB/s\n" "$max"
    echo ""
}


benchmark "MY library" 1
benchmark "GNU coreutils" 2
benchmark "OpenSSL CLI" 3

rm -f test_input.bin test_encoded.b64 test_decoded.bin

