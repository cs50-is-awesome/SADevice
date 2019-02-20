#pragma once
#include "types.h"

void openMemory(dword* at, dword size, dword* policy);
void closeMemory(dword* at, dword size, dword* policy);


class FunctionCallHook {
private:
	dword* target;
	dword original, replacement;
	dword accessRights;
	void openTargetMemory();
	void closeTargetMemory();
public:
	FunctionCallHook(dword* targetCall, dword newFunction);
	void install();
	void uninstall();
};