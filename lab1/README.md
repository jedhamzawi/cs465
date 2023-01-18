# ham-aes

My personal implementation of the AES algorithm written in C. This was completed as a requirement for CS 465 at Brigham Young University.

## Build

The easiest way to build the executable is by running "make" in the root directory. This will require "make" and "gcc" to be installed on your computer. For example:

    jedhamzawi@my-pc ham-aes $ make

This should yield the following output:

    gcc -g -Iinclude src/cipher.c src/finitemath.c src/invcipher.c src/keyexpansion.c src/main.c src/sub.c src/wordmanip.c -o bin/ham-aes 

The binary executable will be output to the "bin" directory inside the project folder.

## Usage
The executable has the following usage pattern:

    ham-aes (-DEBUG) <128/192/256> <enc/dec> <input> <key>

The (-DEBUG) option (also available by specifying "-D" or "-d") will print various information at each step of the algorithm according to the format given in Appendix C of the [AES documentation](https://nvlpubs.nist.gov/nistpubs/fips/nist.fips.197.pdf).

The algorithm can be run with a 128, 192, and 256-bit key length. The given key MUST conform to that specified length, as the program will return an error if the key is too long or too short.

The "enc/dec" option designates whether the algorithm should encrypt or decrypt the input data according to the key.

The input data must be exactly 16 bytes in length and entered in hexadecimal format like the following:

    00112233445566778899aabbccddeeff

The key also follows this same format. This conforms to the official AES documentation listed above.

### Examples

128-bit

    ham-aes 128 enc 00112233445566778899aabbccddeeff 000102030405060708090a0b0c0d0e0f

192-bit

    ham-aes 192 enc 00112233445566778899aabbccddeeff 000102030405060708090a0b0c0d0e0f1011121314151617
    
256-bit

    ham-aes 256 enc 00112233445566778899aabbccddeeff 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f

Debug ON

    ham-aes -DEBUG 128 enc 00112233445566778899aabbccddeeff 000102030405060708090a0b0c0d0e0f

Decrypt

    ham-aes 256 dec 8ea2b7ca516745bfeafc49904b496089 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f

