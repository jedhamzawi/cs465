#include <stdio.h>
#include "finitemath.h"

#define BITS_PER_COLUMN 32

int main(int argc, char *argv[]) {
  printf("0x57 + 0x83 = %x\n", ffAdd(0x57, 0x83));
  printf("xtime(0x57) = %x\n", xtime(0x57));
  printf("0x57 * 0x13 = %x\n", ffMultiply(0x57, 0x13));
}
