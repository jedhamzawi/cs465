#define BLOCK_LENGTH 128
#define WORD_LENGTH 4
#define NB BLOCK_LENGTH / 32

extern unsigned KEY_LENGTH;
extern unsigned NK;
extern unsigned NR;

typedef unsigned char byte;
typedef byte word[WORD_LENGTH];
typedef word state[NB];