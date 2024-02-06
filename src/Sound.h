#pragma once
#include <sys/types.h>

typedef struct VAGsound {
  u_char* VAGfile;
  u_long spuChannel;
  u_long spuAddr;
} VAGsound;

typedef struct VAGhdr {
  char id[4];
  u_int version;
  u_int reserved;
  u_int dataSize;
  u_int samplingFrequency;
  char reserved2[12];
  char name[16];
} VAGhdr;

void initSnd();
u_long sendVAGtoSPU();
void setVoiceAttr(u_int pitch, long channel, u_int soundAddr);
VAGsound createSound(char* filename, u_long channel, u_long spuAddr);
u_long setSPUtransfer(VAGsound* sound);
void playSnd(VAGsound* sound);
