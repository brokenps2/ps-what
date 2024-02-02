#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include <libcd.h>
#include <libpad.h>
#include "CDROM.h"
#include "Graphics.h"

sprite spr;
sprite bg;
sprite clouds;

int pad = 0;

char heap[1024 * 1024];


void init() {

  ResetGraph(0);
  printf("Graph Reset.\n");
  CdInit();
  printf("CD Initialized.\n");
  PadInit(0);
  printf("controller initialized\n");

  InitHeap3(heap, sizeof(heap));

  initGraphics();
}

int main() {

  init();

  clouds = createSprite("\\PSXIMG.TIM;1", 120, 120, 120, 32, 0, 160, 120);
  bg = createSprite("\\SET.TIM;1", 128, 128, 128, 0, 0, 320, 240);
  spr = createSprite("\\WBGND.TIM;1", 128, 128, 128, 64, 64, 64, 64);
  loadTexture(clouds.tfile, &clouds.tim);
  loadTexture(bg.tfile, &bg.tim);
  loadTexture(spr.tfile, &spr.tim);

  if(testCDLoad()) { return 1; }

  while(1) {

    ClearOTagR(ot[db], OTLEN);

    updateSprite(clouds);
    updateSprite(spr);
    updateSprite(bg);

    pad = PadRead(0);
    if(pad & PADLup) spr.y -= 1;
    if(pad & PADLdown) spr.y += 1;
    if(pad & PADLright) spr.x += 1;
    if(pad & PADLleft) spr.x -= 1;

    updateDisplay();

  }

  return 0;

}
