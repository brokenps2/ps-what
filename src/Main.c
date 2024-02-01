#include <stdio.h>
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include <libcd.h>

DISPENV disp[2];
DRAWENV draw[2];
int db;

void display() {
  DrawSync(0);
  VSync(0);
  db = !db;
  PutDispEnv(&disp[db]);
  PutDrawEnv(&draw[db]);
  SetDispMask(1);
}

void init() {

  ResetGraph(0);
  printf("Graph Reset.\n");
  CdInit();
  printf("CD Initialized.\n");

  SetDefDispEnv(&disp[0], 0, 0,   320, 240);
  SetDefDispEnv(&disp[1], 0, 240, 320, 240);

  SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
  SetDefDrawEnv(&draw[1], 0, 0,   320, 240);

  setRGB0(&draw[0], 0, 0, 255);
  setRGB0(&draw[1], 0, 0, 255);
  draw[0].isbg = 1;
  draw[1].isbg = 1;

  printf("Background clear color set.\n");

  PutDispEnv(&disp[0]);
  PutDrawEnv(&draw[0]);

}

int main() {
  init();

  while(1) {

    display();

  }

  return 0;

}
