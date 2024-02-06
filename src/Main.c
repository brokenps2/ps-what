#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include <libcd.h>
#include <libpad.h>
#include <libspu.h>
#include "CDROM.h"
#include "Graphics.h"
#include "Sound.h"

sprite spr;
sprite bg;
int back = 0;

int pad = 0;
int cld = 1;

char heap[1024 * 1024];

VAGsound snd;
VAGsound din;

void init() {

  ResetGraph(0);
  printf("Graph Reset.\n");
  CdInit();
  printf("CD Initialized.\n");
  PadInit(0);
  printf("controller initialized\n");
  SpuInit();
  printf("spu initialized\n");

  InitHeap3(heap, sizeof(heap));

  initGraphics();
  initSnd();

}

int main() {

  init();

  bg = createSprite("\\GDBG.TIM;1", 128, 128, 128, 0, 0, 320, 240);
  spr = createSprite("\\GDPL.TIM;1", 128, 128, 128, 24, 24, 24, 24);
  loadTexture(bg.tfile, &bg.tim);
  loadTexture(spr.tfile, &spr.tim);

  FntLoad(960, 0);
  FntOpen(32, 32, 320, 224, 0, 100);

  if(testCDLoad()) { return 1; }

  
  snd = createSound("\\LC.VAG;1", SPU_0CH, 0);
  snd.spuAddr = setSPUtransfer(&snd);

  din = createSound("\\DING.VAG;1", SPU_1CH, 0);
  din.spuAddr = setSPUtransfer(&din);

  while(1) {

    ClearOTagR(ot[db], OTLEN);

    updateSprite(spr);
    updateSprite(bg);

    pad = PadRead(0);
    if(pad & PADLup) spr.y -= 2;
    if(pad & PADLdown) spr.y += 2;
    if(pad & PADLright) spr.x += 2;
    if(pad & PADLleft) spr.x -= 2;
    if(pad & PADLright) playSnd(&snd);
    if(pad & PADLleft) playSnd(&din);

    FntPrint("gdpsx");

    FntFlush(-1);

    updateDisplay();

  }

  return 0;

}
