#include <sys/types.h>
#include <stdio.h>
#include <libspu.h>
#include "CDROM.h"
#include "Sound.h"

#define VAG_NBR 8
#define MALLOC_MAX VAG_NBR
#define SWAP_ENDIAN32(x) (((x)>>24) | (((x)>>8) & 0xFF00) | (((x)<<8) & 0x00FF0000) | ((x)<<24))

char spuMallocRec[SPU_MALLOC_RECSIZ * (MALLOC_MAX + 1)];

SpuCommonAttr commonAttribs;
SpuVoiceAttr voiceAttribs;

void initSnd() {
  SpuInitMalloc(MALLOC_MAX, spuMallocRec);
  commonAttribs.mask = (SPU_COMMON_MVOLL | SPU_COMMON_MVOLR);
  commonAttribs.mvol.left = 0x3fff;
  commonAttribs.mvol.right = 0x3fff;
  SpuSetCommonAttr(&commonAttribs);
  SpuSetIRQ(SPU_OFF);
  SpuSetKey(SpuOff, SPU_ALLCH);
}

u_long sendVAGtoSPU(u_int VAGdataSize, u_char* VAGdata) {
  u_long transferred;
  SpuSetTransferMode(SpuTransByDMA);
  transferred = SpuWrite(VAGdata + sizeof(VAGhdr), VAGdataSize);
  SpuIsTransferCompleted(SPU_TRANSFER_WAIT);
  return transferred;
}

void setVoiceAttr(u_int pitch, long channel, u_int soundAddr) {
  voiceAttribs.mask=
    (
      SPU_VOICE_VOLL |
      SPU_VOICE_VOLR |
      SPU_VOICE_PITCH |
      SPU_VOICE_WDSA |
      SPU_VOICE_ADSR_AMODE |
      SPU_VOICE_ADSR_SMODE |
      SPU_VOICE_ADSR_RMODE |
      SPU_VOICE_ADSR_AR |
      SPU_VOICE_ADSR_DR |
      SPU_VOICE_ADSR_SR |
      SPU_VOICE_ADSR_RR |
      SPU_VOICE_ADSR_SL
    );
  voiceAttribs.voice = channel;
  voiceAttribs.volume.left = 0x0;
  voiceAttribs.volume.right = 0x0;
  voiceAttribs.pitch = pitch;
  voiceAttribs.addr = soundAddr;
  voiceAttribs.a_mode = SPU_VOICE_LINEARIncN;
  voiceAttribs.s_mode = SPU_VOICE_LINEARIncN;
  voiceAttribs.r_mode = SPU_VOICE_LINEARDecN;
  voiceAttribs.ar = 0x0;
  voiceAttribs.dr = 0x0;
  voiceAttribs.rr = 0x0;
  voiceAttribs.sr = 0x0;
  voiceAttribs.sl = 0xf;
  SpuSetVoiceAttr(&voiceAttribs);
}

VAGsound createSound(char* filename, u_long channel, u_long spuAddr) {

  VAGsound sound;
  sound.VAGfile = loadFileFromCD(filename);
  sound.spuChannel = channel;
  sound.spuAddr = spuAddr;

  return sound;

}

u_long setSPUtransfer(VAGsound* sound) {
  u_long transferred, spuAddr;
  u_int pitch;
  const VAGhdr* vagHeader = (VAGhdr*)sound->VAGfile;
  pitch = (SWAP_ENDIAN32(vagHeader->samplingFrequency) << 12) /44100L;
  spuAddr = SpuMalloc(SWAP_ENDIAN32(vagHeader->dataSize));
  SpuSetTransferStartAddr(spuAddr);
  transferred = sendVAGtoSPU(SWAP_ENDIAN32(vagHeader->dataSize), sound->VAGfile);
  setVoiceAttr(pitch, sound->spuChannel, spuAddr);
  return spuAddr;
}

void playSnd(VAGsound* sound) {
  voiceAttribs.mask = (SPU_VOICE_VOLL | SPU_VOICE_VOLR);
  voiceAttribs.voice = sound->spuChannel;
  voiceAttribs.volume.left = 0x1000;
  voiceAttribs.volume.right = 0x1000;
  SpuSetVoiceAttr(&voiceAttribs);
  SpuSetKey(SpuOn, sound->spuChannel);
}
