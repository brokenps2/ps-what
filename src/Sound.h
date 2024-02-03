#pragma once
#include <sys/types.h>

#define SWAP_ENDIAN32(x) (((x)>>24) | (((x)>>8) & 0xFF00) | (((x)<<8) & 0x00FF0000) | ((x)<<24))

typedef struct VAGheader{       // All the values in this header must be big endian
  char id[4];             // VAGp         4 bytes -> 1 char * 4
  unsigned int version;          // 4 bytes
  unsigned int reserved;         // 4 bytes
  unsigned int dataSize;         // (in bytes) 4 bytes
  unsigned int samplingFrequency;// 4 bytes
  char reserved2[12];    // 12 bytes -> 1 char * 12
  char name[16];         // 16 bytes -> 1 char * 16
  // Waveform data after that
}VAGhdr;

//void initSnd();
u_long setVAGtoRAM();
void setVoiceAttr();
void playSFX();
void initSound();
void loadSound(char* filename, int pitchmod, long channel, int adomu);
void playLoadedSnd();
