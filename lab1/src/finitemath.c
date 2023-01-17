#ifndef TYPES_H
#define TYPES_H

#include "finitemath.h"

#define AES_MOD_POLY 0x011b

byte ffAdd(const byte field1, const byte field2) {
  return field1 ^ field2;
}

int isKthBitSet(const unsigned short num, const int k) {
  return num & (1 << k);
}

byte xtime(const byte field) {
  return isKthBitSet(field << 1, 8) ? (field << 1) ^ AES_MOD_POLY : field << 1;
}

byte ffMultiply(const byte field1, const byte field2) {
  byte total = 0;
  byte term = 0;
  for (unsigned char bitIndex = 0; bitIndex < 7; bitIndex++) {
    if (isKthBitSet(field2, bitIndex)) {
      term = field1;
      for(unsigned char i = 0; i < bitIndex; i++) {
        term = xtime(term);
      }
      total = total ^ term;
    }
  }

  return total;;
}

#endif