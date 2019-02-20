#include "hooking.h"

#include <Windows.h>


void openMemory(dword* at, dword size, dword* policy) {
	VirtualProtect(at, size, PAGE_EXECUTE_READWRITE, (PDWORD)policy);
}

void closeMemory(dword* at, dword size, dword* policy) {
	VirtualProtect(at, size, *policy, (PDWORD)policy);
}

FunctionCallHook::FunctionCallHook(dword* targetCall, dword newFunction) {
	target = targetCall;
	replacement = newFunction;
}

void FunctionCallHook::openTargetMemory() {
	openMemory(target, 4, &accessRights);
}

void FunctionCallHook::closeTargetMemory() {
	closeMemory(target, 4, &accessRights);
}

void FunctionCallHook::install() {
	dword hookOffset = replacement - (dword)target - 4;

	// Open access to the area in memory that is otherwise read only
	openTargetMemory();

	original = *target;
	*target = hookOffset;

	// Restore the old memory policy rights
	closeTargetMemory();
}

void FunctionCallHook::uninstall() {
	openTargetMemory();
	*target = original;
	closeTargetMemory();
}