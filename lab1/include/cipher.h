#include "global.h"

void cipher(byte in[4*NB], byte out[4*NB], word w[NB*(NR+1)]);
void inToState(byte in[WORD_LENGTH*NB], state state);
void stateToOut(state state, byte out[WORD_LENGTH*NB]);
void subBytes(state state);
void shiftRows(state state);
void mixColumns(state state);
void setCol(state state, word newCol, unsigned col);
void addRoundKey(state state, word w[NB*(NR+1)], unsigned round);
