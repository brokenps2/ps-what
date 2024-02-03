#include <libsnd.h>
#include <libspu.h>
#include "Sound.h"
#include "CDROM.h"

#define MALLOC_MAX 3

SpuCommonAttr commonAttributes;          // structure for changing common voice attributes
SpuVoiceAttr  voiceAttributes ;          // structure for changing individual voice attributes
u_long vag_spu_address;

u_long spu_start_address;                
u_long get_start_addr;
u_long transSize;

long channelglobal = 0;
int pitchmodglobal = 0;
int adomuglobal = 1;

char spu_malloc_rec[SPU_MALLOC_RECSIZ * (MALLOC_MAX+1)];

u_char* sndfile;

void initSnd() {
  SpuInitMalloc(MALLOC_MAX, spu_malloc_rec);
  commonAttributes.mask = (SPU_COMMON_MVOLL | SPU_COMMON_MVOLR);
  commonAttributes.mvol.left = 0x3fff;
  commonAttributes.mvol.right = 0x3fff;
  SpuSetCommonAttr(&commonAttributes);
  SpuSetIRQ(SPU_OFF);
}

u_long sendVAGtoRAM(u_int VAG_data_size, u_char *VAG_data) {
  u_long size;
  SpuSetTransferMode(SpuTransByDMA);
  size = SpuWrite(VAG_data + sizeof(VAGhdr), VAG_data_size);
  SpuIsTransferCompleted(SPU_TRANSFER_WAIT);
  return size;
}

void setVoiceAttr(u_int pitch, long channel, u_long soundAddr) {
  voiceAttributes.mask = (
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

  voiceAttributes.voice = channel;
  voiceAttributes.volume.left = 0x1000;
  voiceAttributes.volume.right = 0x1000;
  voiceAttributes.pitch = pitch;
  voiceAttributes.addr = soundAddr;
  voiceAttributes.a_mode = SPU_VOICE_LINEARIncN;
  voiceAttributes.s_mode = SPU_VOICE_LINEARIncN;
  voiceAttributes.r_mode = SPU_VOICE_LINEARDecN;
  voiceAttributes.ar = 0x0;
  voiceAttributes.dr = 0x0;
  voiceAttributes.rr = 0x0;
  voiceAttributes.sr = 0x0;
  voiceAttributes.sl = 0xf;
  SpuSetVoiceAttr(&voiceAttributes);

}

void playSFX(){
  SpuSetKey(SpuOn,channelglobal);
}

void initSound() {
  SpuInit();
  initSnd();

}

void setPitchMod(int pmd) {
  pitchmodglobal = pmd;
}

void setAdomu(int adomu) {
  adomuglobal = adomu;
}

void setChannel(long chan) {
  channelglobal = chan;
}

void loadSound(char* filename, int pitchmod, long chan, int adomu) {

  sndfile = loadFileFromCD(filename);

  short counter = 0;
  const VAGhdr* VAGFileHeader = (VAGhdr*)sndfile;
  u_int pitch = (SWAP_ENDIAN32(VAGFileHeader->samplingFrequency) << 12) / 44100L;

  vag_spu_address = SpuMalloc(SWAP_ENDIAN32(VAGFileHeader->dataSize));
  spu_start_address = SpuSetTransferStartAddr(vag_spu_address);
  get_start_addr = SpuGetTransferStartAddr();
  transSize = sendVAGtoRAM(SWAP_ENDIAN32(VAGFileHeader->dataSize), sndfile);

  channelglobal = chan;
  adomuglobal = adomu;
  pitchmodglobal = pitchmod;

  if(adomuglobal) {
    setVoiceAttr(pitch + pitchmodglobal, channelglobal, vag_spu_address);
  } else {
    setVoiceAttr(pitch * pitchmodglobal, channelglobal, vag_spu_address);
  }


}

void playLoadedSnd() {

  playSFX();

}
