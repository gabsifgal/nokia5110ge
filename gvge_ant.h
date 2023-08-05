//#ifndef GVGE_ANT_H
//#define GVGE_ANT_H
#pragma once
#include <stdlib.h>
#include <stdint.h>
#include "gvge.h"

//const int W = 84;
//constint H = 48;

const extern int W;
const extern int H;

struct Ant;

typedef struct Ant* ANT;

ANT ANT__create(int x, int y);

int ANT__x(ANT self);
int ANT__y(ANT self);

void ANT__draw(ANT self, uint8_t scrn[W][H]);

void ANT__update(ANT self, uint8_t scrn[W][H]);

void GVGE_ANT__INITDISPLAY(uint8_t arr[W][H]);

void GVGE_ANT__SHOWDISPLAY(uint8_t arr[W][H]);

//#endif /* GVGE_ANT_H */