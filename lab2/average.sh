#!/bin/bash

attack=$1
bits=$2
iters=$3
total=0
for (( i = 0; i < $iters; i++ ))
do
    attempts=$(./hash-attack $attack $bits | grep "Found match in" | tr -d -c 0-9)
    total=$((total+attempts))
    echo $attempts
done

echo "Total: $total"
echo "Average: $((total/iters))"

