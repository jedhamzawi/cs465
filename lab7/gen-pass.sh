#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

pass=$1
salt=$(echo $RANDOM | md5sum | head -c 20; echo;)
hash=$(openssl passwd -1 -salt $salt $pass)

echo "bob:$hash::::::" | tee shadow-$pass
../unshadow passwd-template shadow-$pass > passwd-$pass
