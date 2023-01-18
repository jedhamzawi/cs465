#ifndef GLOBAL_H
#define GLOBAL_H

#include "wordmanip.h"
#include "sub.h"

void rotWord(word word) {
  byte temp = word[0];
  word[0] = word[1];
  word[1] = word[2];
  word[2] = word[3];
  word[3] = temp;
}

void invRotWord(word word) {
  byte temp = word[3];
  word[3] = word[2];
  word[2] = word[1];
  word[1] = word[0];
  word[0] = temp;
}

void subWord(word word) {
  for (unsigned i = 0; i < WORD_LENGTH; i++) {
    word[i] = getSubByte(word[i]);
  }
}

// Set word1 equal to word2
void setWord(word word1, word word2) {
  for (unsigned i = 0; i < WORD_LENGTH; i++) {
    word1[i] = word2[i];
  }
}

void xOrWords(word word1, word word2, word result) {
  for (unsigned i = 0; i < WORD_LENGTH; i++) {
    result[i] = word1[i] ^ word2[i];
  }
}

#endif
