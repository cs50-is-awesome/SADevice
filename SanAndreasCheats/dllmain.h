#pragma once
#include "d3dhook.h"

void init();
void hook();
void hack();
void unload();

VOID WINAPI paint(LPDIRECT3DDEVICE9 pDevice);