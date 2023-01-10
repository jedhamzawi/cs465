#include "finitemath.h"

#define AES_MOD_POLY 0x011b

FIELD ffAdd(const FIELD field1, const FIELD field2) {
  return field1 ^ field2;
}

int isKthBitSet(const unsigned short num, const int k) {
  return num & (1 << k);
}

FIELD xtime(const FIELD field) {
  return isKthBitSet(field << 1, 8) ? (field << 1) ^ AES_MOD_POLY : field << 1;
}

FIELD ffMultiply(const FIELD field1, const FIELD field2) {
  FIELD total = 0;
  FIELD term = 0;
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