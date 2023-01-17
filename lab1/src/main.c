#include <stdio.h>
#include <stdlib.h>
#include "finitemath.h"
#include "keyexpansion.h"

#define BITS_PER_COLUMN 32

unsigned KEY_LENGTH;
unsigned NK;
unsigned NR;

void printBytes(byte *buf, unsigned size);
void usage();

//DEFINE Nk (key length) and Nr (num rounds) based on key size
int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("ERROR: Key length not provided\n");
    usage();
    return 1;
  }
  
  KEY_LENGTH = strtol(argv[1], NULL, 10);
  if (KEY_LENGTH != 128 && KEY_LENGTH != 192 && KEY_LENGTH != 256) {
    printf("ERROR: Invalid key length\n");
    usage();
    return 1;
  }
  NK = KEY_LENGTH / 32;
  NR = NK + 6;
  printf("0x57 + 0x83 = %x\n", ffAdd(0x57, 0x83));
  printf("xtime(0x57) = %x\n", xtime(0x57));
  printf("0x57 * 0x13 = %x\n", ffMultiply(0x57, 0x13));

  byte word1[4] = {0x09, 0xcf, 0x4f, 0x3c};
  rotWord(word1);
  printBytes(word1, 4);

  byte word2[4] = {0x00, 0x10, 0x20, 0x30};
  subWord(word2);
  printBytes(word2, 4);

  byte key[16] = {0x2b, 0x7e, 0x15, 0x16,
                  0x28, 0xae, 0xd2, 0xa6,
                  0xab, 0xf7, 0x15, 0x88,
                  0x09, 0xcf, 0x4f, 0x3c};
  word w[NB*(NR+1)];
  keyExpansion(key, w);

}

void usage() {
 printf("Usage: ham-aes [128/192/256]\n");
}

void printBytes(byte *buf, unsigned size) {
  printf("0x");
  for (unsigned i = 0; i < size; i++)
  {
      printf("%02x", buf[i]);
  }
  printf("\n");
}
