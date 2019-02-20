#pragma once

#include "types.h"

#include <d3d9.h>
#include <d3dx9.h>

#include <Windows.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

typedef HRESULT(WINAPI* oEndScene) (LPDIRECT3DDEVICE9 D3DDevice);

static oEndScene originalEndScene;

bool match(const byte* pData, const byte* bMask, const char* szMask);

DWORD find_pattern(dword dwAddress, dword dwLen, byte *bMask, char * szMask);

void hookEndScene();