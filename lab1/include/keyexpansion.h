#include "types.h"

void keyExpansion(byte key[4*NK], byte w[4][NB*(NR+1)]);
void rotWord(byte word[]);
void subWord(byte word[]);
void setWord(word word1, word word2);
void xOrWords(word word1, word word2, word result);
void getRCon(unsigned char round, word rCon);
void subBytes(byte [][NB]);