#pragma once
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>

#define OTLEN 8

extern u_long ot[2][OTLEN];
extern int db;

typedef struct sprite {
  u_long* tfile;
  SPRT* sprt;
  DR_TPAGE* tpage;
  TIM_IMAGE tim;
  int r;
  int g;
  int b;
  int x;
  int y;
  int w;
  int h;
} sprite;

void initGraphics();

void updateDisplay();

void loadTexture(u_long* timf, TIM_IMAGE* timg);

sprite createSprite(char* path, int r, int g, int b, int x, int y, int w, int h);

u_long* getOTDB();

void updateSprite(sprite sprite);


