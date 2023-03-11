#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

filepath=$1

rm ../john.pot 2> /dev/null
../john --format=md5crypt-long $filepath 
