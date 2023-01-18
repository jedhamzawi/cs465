#include "global.h"

void invCipher(byte in[4*NB], byte out[4*NB], word w[NB*(NR+1)]);
void invSubBytes(state state);
void invShiftRows(state state);
void invMixColumns(state state);
