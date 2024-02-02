#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include <libcd.h>
#include "CDROM.h"

#define OTLEN 8

DISPENV disp[2];
DRAWENV draw[2];
u_long ot[2][OTLEN];
char* primbuff[2][32768];
char* nextpri = primbuff[0];
int db;

u_long* timf;
TIM_IMAGE timg;

char heap[1024 * 1024];

void loadTexture(u_long* timf, TIM_IMAGE* timg) {
  OpenTIM(timf);
  ReadTIM(timg);
  LoadImage(timg->prect, timg->paddr);
  DrawSync(0);
  if(timg->mode * 0x8) {
    LoadImage(timg->crect, timg->caddr);
    DrawSync(0);
  }
}

void display() {
  DrawSync(0);
  VSync(0);
  PutDispEnv(&disp[db]);
  PutDrawEnv(&draw[db]);
  SetDispMask(1);
  DrawOTag(&ot[db][OTLEN - 1]);
  db = !db;
  nextpri = primbuff[db];
}

void init() {

  ResetGraph(0);
  printf("Graph Reset.\n");
  CdInit();
  printf("CD Initialized.\n");

  InitHeap3(heap, sizeof(heap));

  SetDefDispEnv(&disp[0], 0, 0,   320, 240);
  SetDefDispEnv(&disp[1], 0, 240, 320, 240);

  SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
  SetDefDrawEnv(&draw[1], 0, 0,   320, 240);

  setRGB0(&draw[0], 0, 0, 0);
  setRGB0(&draw[1], 0, 0, 0);
  draw[0].isbg = 1;
  draw[1].isbg = 1;

  printf("Background clear color set.\n");

  PutDispEnv(&disp[0]);
  PutDrawEnv(&draw[0]);

  timf = loadFileFromCD("\\PSXIMG.TIM;1");

}

int main() {

  init();

  if(testCDLoad()) { return 1; }

  SPRT* sprite;
  DR_TPAGE* tpage;

  loadTexture(timf, &timg);

  while(1) {

    ClearOTagR(ot[db], OTLEN);
    sprite = (SPRT*)nextpri;
    setSprt(sprite);
    setRGB0(sprite, 128, 128, 128);
    setXY0(sprite, 28, 28);
    setWH(sprite, 160, 120);
    setClut(sprite, timg.crect->x, timg.crect->y);
    addPrim(ot[db], sprite);
    nextpri += sizeof(SPRT);

    tpage = (DR_TPAGE*)nextpri;
    setDrawTPage(tpage, 0, 1, getTPage(timg.mode&0x3, 0, timg.prect->x, timg.prect->y));
    addPrim(ot[db], tpage);                 
    nextpri += sizeof(DR_TPAGE);

    display();

  }

  return 0;

}
