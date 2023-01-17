#include "keyexpansion.h"
#include "sub.h"
#include "finitemath.h"

void keyExpansion(byte key[WORD_LENGTH*NK], word w[NB*(NR+1)]) {
  for (unsigned i = 0; i < NK; i++) {
    setWord(w[i], (word) {key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]});
  }

  word temp;
  word rCon;
  word result;
  for (unsigned i = NK; i < (NB*(NR+1)); i++) {
    setWord(temp, w[i-1]);
    if (i % NK == 0) {
      rotWord(temp);
      subWord(temp);
      getRCon(i/NK, rCon);
      xOrWords(temp, rCon, result);
      setWord(temp, result);
    }
    else if (NK > 6 && i % NK == 4) {
      subWord(temp);
    }
    xOrWords(w[i-NK], temp, result);
    setWord(w[i], result);
  }
}

void rotWord(word word) {
  byte temp = word[0];
  word[0] = word[1];
  word[1] = word[2];
  word[2] = word[3];
  word[3] = temp;
}

void subWord(word word) {
  for (unsigned i = 0; i < WORD_LENGTH; i++) {
    word[i] = subByte(word[i]);
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

void getRCon(unsigned char round, word rCon) {
  byte x = 0x02;
  for (unsigned char i = 2; i < round; i++) {
    x = xtime(x);
  }
  rCon[0] = round == 1 ? 0x01 : x;
  rCon[1] = 0x00;
  rCon[2] = 0x00;
  rCon[3] = 0x00;
}
