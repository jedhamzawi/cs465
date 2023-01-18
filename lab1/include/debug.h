#include "global.h"

extern unsigned char DEBUG;

#define debug_print(fmt, ...) \
  do { if (DEBUG) printf(fmt, __VA_ARGS__); } while (0)

#define debug_printBytes(buf, size) \
  do { if (DEBUG) { \
    for (unsigned i = 0; i < size; i++) { printf("%02x", buf[i]); } \
    printf("\n"); \
  }} while (0)

#define debug_printState(state) \
  do { if (DEBUG) { \
    for (unsigned row = 0; row < WORD_LENGTH; row++) { \
      for (unsigned col = 0; col < NB; col++) { \
        printf("%02x", state[col][row]); }} \
    printf("\n"); \
  }} while (0)

#define debug_printRoundKey(w, round) \
  do { if (DEBUG) { \
    for (unsigned i = 0; i < WORD_LENGTH * NB; i++) { \
      printf("%02x", w[round*NB][i]); } \
    printf("\n"); \
  }} while (0)
