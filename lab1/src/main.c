#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "finitemath.h"
#include "keyexpansion.h"
#include "wordmanip.h"
#include "cipher.h"
#include "invcipher.h"
#include "debug.h"

#define BITS_PER_COLUMN 32
#define DATA_SIZE_BYTES 16

#define BAD_USAGE_ERROR 1
#define INVALID_SIZE_ERROR 2

unsigned ENC = 1;

unsigned KEY_LENGTH;
unsigned NK;
unsigned NR;
unsigned char DEBUG;

void printBytes(byte *buf, unsigned size);
void usage();
void hexStringToBytes(char hex[], byte bytes[], unsigned size);

int main(int argc, char *argv[]) {
  if (argc < 5) {
    printf("ERROR: Invalid number of arguments\n");
    usage();
    return BAD_USAGE_ERROR;
  }

  unsigned encDecIndex = 1;
  unsigned keyLengthIndex = 2;
  unsigned inIndex = 3;
  unsigned keyIndex = 4;
  if (argc == 6) {
    if (strcmp(argv[1], "-DEBUG") != 0 &&
        strcmp(argv[1], "-d") != 0 &&
        strcmp(argv[1], "-D") != 0) {
      printf("ERROR: Too many arguments. To enable debugging, -DEBUG flag must immediately follow program call.\n");
      usage();
      return BAD_USAGE_ERROR;
    }
    DEBUG = 1;
    encDecIndex++;
    keyLengthIndex++;
    inIndex++;
    keyIndex++;
  }
  else {
    DEBUG = 0;
  }

  if (strcmp(argv[encDecIndex], "dec") == 0 ||
      strcmp(argv[encDecIndex], "DEC") == 0) {
    ENC = 0;
  }
  else if (strcmp(argv[encDecIndex], "enc") != 0 &&
          strcmp(argv[encDecIndex], "ENC") == 0) {
    printf("ERROR: Invalid encryption/decryption option.");
    usage();
    return BAD_USAGE_ERROR;
  }
  
  KEY_LENGTH = strtol(argv[keyLengthIndex], NULL, 10);
  if (KEY_LENGTH != 128 && KEY_LENGTH != 192 && KEY_LENGTH != 256) {
    printf("ERROR: Unsupported key length\n");
    usage();
    return BAD_USAGE_ERROR;
  }
  NK = KEY_LENGTH / 32;
  NR = NK + 6;

  if (strlen(argv[inIndex]) != DATA_SIZE_BYTES * 2) {
    printf("ERROR: Input data must have exactly 32 hex digits (16 bytes).\n");
    return INVALID_SIZE_ERROR;
  }
  if (strlen(argv[keyIndex]) != KEY_LENGTH / 4) {
    printf("ERROR: The given key does not match the specified length.\n"
      "A key length of %d bits requires a key with %d digits\nGiven: %d\n",
      KEY_LENGTH, KEY_LENGTH / 4, (int) strlen(argv[3]));
      return INVALID_SIZE_ERROR;
  }

  byte in[DATA_SIZE_BYTES];
  byte key[KEY_LENGTH / 8];

  debug_print("PLAINTEXT: %s\nKEY: %s\n\n", argv[inIndex], argv[keyIndex]);
  hexStringToBytes(argv[inIndex], in, sizeof in);
  hexStringToBytes(argv[keyIndex], key, sizeof key);

  word w[NB*(NR+1)];
  
  keyExpansion(key, w);
  
  byte out[16];

  if (ENC) {
    cipher(in, out, w);
  }
  else {
    invCipher(in, out, w);
  }
}

void usage() {
 printf("  Usage: ham-aes <enc/dec> (-DEBUG) <128/192/256> <data> <key>\n");
}

void hexStringToBytes(char hex[], byte bytes[], unsigned size) {
  for (unsigned count = 0; count < size/sizeof *bytes; count++) {
        sscanf(hex, "%2hhx", &bytes[count]);
        hex += 2;
    }
}

#endif
