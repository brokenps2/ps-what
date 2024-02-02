#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include "CDROM.h"
#include "Graphics.h"

DISPENV disp[2];
DRAWENV draw[2];
u_long ot[2][OTLEN];
char* primbuff[2][32768];
char* nextpri = primbuff[0];
int db;

void initGraphics() {
  
  SetDefDispEnv(&disp[0], 0, 0,   320, 240);
  SetDefDispEnv(&disp[1], 0, 240, 320, 240);

  SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
  SetDefDrawEnv(&draw[1], 0, 0,   320, 240);

  setRGB0(&draw[0], 0, 0, 0);
  setRGB0(&draw[1], 0, 0, 0);
  draw[0].isbg = 1;
  draw[1].isbg = 1;

  PutDispEnv(&disp[0]);
  PutDrawEnv(&draw[0]);

}

void updateDisplay() {
  DrawSync(0);
  VSync(0);
  PutDispEnv(&disp[db]);
  PutDrawEnv(&draw[db]);
  SetDispMask(1);
  DrawOTag(&ot[db][OTLEN - 1]);
  db = !db;
  nextpri = primbuff[db];
}

sprite createSprite(char* path, int r, int g, int b, int x, int y, int w, int h) {
  
  sprite spr;

  spr.tfile = loadFileFromCD(path);
  spr.r = r;
  spr.g = g;
  spr.b = b;
  spr.x = x;
  spr.y = y;
  spr.w = w;
  spr.h = h;

  return spr;
}

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

void updateSprite(sprite sprite) {
  sprite.sprt = (SPRT*)nextpri;
  setSprt(sprite.sprt);
  setRGB0(sprite.sprt, sprite.r, sprite.g, sprite.b);
  setXY0(sprite.sprt, sprite.x, sprite.y);
  setWH(sprite.sprt, sprite.w, sprite.h);
  setClut(sprite.sprt, sprite.tim.crect->x, sprite.tim.crect->y);
  addPrim(ot[db], sprite.sprt);
  nextpri += sizeof(SPRT);

  sprite.tpage = (DR_TPAGE*)nextpri;
  setDrawTPage(sprite.tpage, 0, 1, getTPage(sprite.tim.mode&0x3, 0, 
                                            sprite.tim.prect->x, sprite.tim.prect->y));
  addPrim(ot[db], sprite.tpage);
  nextpri += sizeof(DR_TPAGE);
}

