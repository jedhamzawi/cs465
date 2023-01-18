#ifndef GLOBAL_H
#define GLOBAL_H

#include "invcipher.h"
#include "cipher.h"
#include "sub.h"
#include "wordmanip.h"
#include "finitemath.h"
#include "debug.h"

const word INV_MIX_MATRIX[NB] = {
  {0x0e, 0x0b, 0x0d, 0x09},
  {0x09, 0x0e, 0x0b, 0x0d},
  {0x0d, 0x09, 0x0e, 0x0b},
  {0x0b, 0x0d, 0x09, 0x0e}
};

void invCipher(byte in[4*NB], byte out[4*NB], word w[NB*(NR+1)]) {
  debug_print("INVERSE CIPHER (DECRYPT):\nround[ %d].iinput ", 0);
  debug_printBytes(in, 4*NB);
  state state;
  inToState(in, state);
  debug_print("round[ %d].ik_sch ", 0);
  addRoundKey(state, w, NR);
  unsigned iround = 1;
  for (unsigned round = NR-1; round > 0; round--){
    debug_print("round[ %d].istart ", iround);
    debug_printState(state);
    invShiftRows(state);
    debug_print("round[ %d].is_row ", iround);
    debug_printState(state);
    invSubBytes(state);
    debug_print("round[ %d].is_box ", iround);
    debug_printState(state);
    debug_print("round[ %d].ik_sch ", iround);
    addRoundKey(state, w, round);
    invMixColumns(state);
    debug_print("round[ %d].im_col ", iround);
    debug_printState(state);

    iround++;
  }
  debug_print("round[ %d].istart ", NR);
  debug_printState(state);
  invShiftRows(state);
  debug_print("round[ %d].is_row ", NR);
  debug_printState(state);
  invSubBytes(state);
  debug_print("round[ %d].is_box ", NR);
  debug_printState(state);
  debug_print("round[ %d].ik_sch ", NR);
  addRoundKey(state, w, 0);
  debug_print("round[ %d].ioutput ", NR);
  debug_printState(state);
  stateToOut(state, out);
}

void invSubBytes(state state) {
  for (unsigned row = 0; row < WORD_LENGTH; row++) {
    for (unsigned col = 0; col < NB; col++) {
      state[row][col] = getInvSubByte(state[row][col]);
    }
  }
}

void invShiftRows(state state) {
  for (unsigned row = 1; row < WORD_LENGTH; row++) {
    for (unsigned i = 0; i < row; i++) {
      invRotWord(state[row]);
    }
  }
}

void invMixColumns(state state) {
  for (unsigned col = 0; col < NB; col++) {
    word newCol;
    for (unsigned newColRow = 0; newColRow < NB; newColRow++) {
      byte total = 0;
      for (unsigned row = 0; row < WORD_LENGTH; row++) {
        total = ffAdd(total, ffMultiply(state[row][col], INV_MIX_MATRIX[newColRow][row]));
      }
      newCol[newColRow] = total;
    }
    setCol(state, newCol, col);
  }
}


#endif
