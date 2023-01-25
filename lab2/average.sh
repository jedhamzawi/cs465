#!/bin/zsh

ATTACK=$1
BITS=$2
ITERS=$3
TOTAL=0
for i in {1..$ITERS}
do
    ATTEMPTS=$(./hash-attack $ATTACK $BITS | grep "Found match in" | tr -d -c 0-9)
    TOTAL=$(($TOTAL + $ATTEMPTS))
    echo $ATTEMPTS
done

echo "Total: $TOTAL"
echo "Average: $(($TOTAL / $ITERS))"
