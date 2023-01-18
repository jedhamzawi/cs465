#ifndef GLOBAL_H
#define GLOBAL_H

#include "cipher.h"
#include "sub.h"
#include "wordmanip.h"
#include "finitemath.h"
#include "debug.h"

const word MIX_MATRIX[NB] = {
  {0x02, 0x03, 0x01, 0x01},
  {0x01, 0x02, 0x03, 0x01},
  {0x01, 0x01, 0x02, 0x03},
  {0x03, 0x01, 0x01, 0x02}
};

void cipher(byte in[4*NB], byte out[4*NB], word w[NB*(NR+1)]) {
  debug_print("CIPHER (ENCRYPT):\nround[ %d].input ", 0);
  debug_printBytes(in, 4*NB);
  state state;
  inToState(in, state);
  debug_print("round[ %d].k_sch ", 0);
  addRoundKey(state, w, 0);
  for (unsigned round = 1; round < NR; round++){
    debug_print("round[ %d].start ", round);
    debug_printState(state);
    subBytes(state);
    debug_print("round[ %d].s_box ", round);
    debug_printState(state);
    shiftRows(state);
    debug_print("round[ %d].s_row ", round);
    debug_printState(state);
    mixColumns(state);
    debug_print("round[ %d].m_col ", round);
    debug_printState(state);
    debug_print("round[ %d].k_sch ", round);
    addRoundKey(state, w, round);
  }
  debug_print("round[ %d].start ", NR);
  debug_printState(state);
  subBytes(state);
  debug_print("round[ %d].s_box ", NR);
  debug_printState(state);
  shiftRows(state);
  debug_print("round[ %d].s_row ", NR);
  debug_printState(state);
  debug_print("round[ %d].k_sch ", NR);
  addRoundKey(state, w, NR);

  debug_print("round[ %d].output ", NR);
  debug_printState(state);
  stateToOut(state, out);
}

void inToState(byte in[WORD_LENGTH*NB], state state) {
  for (unsigned row = 0; row < WORD_LENGTH; row++) {
    for (unsigned col = 0; col < NB; col++) {
      state[row][col] = in[row+(4*col)];
    }
  }
}

void stateToOut(state state, byte out[WORD_LENGTH*NB]) {
  for (unsigned row = 0; row < WORD_LENGTH; row++) {
    for (unsigned col = 0; col < NB; col++) {
      out[row+(4*col)] = state[row][col];
    }
  }
}

void subBytes(state state) {
  for (unsigned row = 0; row < WORD_LENGTH; row++) {
    for (unsigned col = 0; col < NB; col++) {
      state[row][col] = getSubByte(state[row][col]);
    }
  }
}

void shiftRows(state state) {
  for (unsigned row = 1; row < WORD_LENGTH; row++) {
    for (unsigned i = 0; i < row; i++) {
      rotWord(state[row]);
    }
  }
}

void mixColumns(state state) {
  for (unsigned col = 0; col < NB; col++) {
    word newCol;
    for (unsigned newColRow = 0; newColRow < NB; newColRow++) {
      byte total = 0;
      for (unsigned row = 0; row < WORD_LENGTH; row++) {
        total = ffAdd(total, ffMultiply(state[row][col], MIX_MATRIX[newColRow][row]));
      }
      newCol[newColRow] = total;
    }
    setCol(state, newCol, col);
  }
}

void setCol(state state, word newCol, unsigned col) {
  for (unsigned row = 0; row < WORD_LENGTH; row++) {
    state[row][col] = newCol[row];
  }
}

void addRoundKey(state state, word w[NB*(NR+1)], unsigned round) {
  debug_printRoundKey(w, round);
  for (unsigned row = 0; row < WORD_LENGTH; row++) {
    for (unsigned col = 0; col < NB; col++) {
      state[row][col] = ffAdd(state[row][col], w[round*NB+col][row]);
    }
  }
}


#endif
