#include "Includes.h"

void Patch(BYTE* dst, BYTE* src, unsigned int size) {
	DWORD oProc;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oProc);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oProc, &oProc);
}

bool Hook(char* src, char* dst, int len) {
	if (len < 5) return false;
	DWORD oldprotect;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(src, 0x90, len);
	uintptr_t realAddy = (uintptr_t)(dst - src - 5);
	*src = (char)0xe9;
	*(uintptr_t*)(src + 1) = (uintptr_t)realAddy;
	VirtualProtect(src, len, oldprotect, &oldprotect);
}

char* TrampHook(char* src, char* dst, unsigned int len) {
	if (len < 5) return 0;

	char* gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(gateway, src, len);
	uintptr_t jumpAddy = (uintptr_t)(src - gateway - 5);
	*(gateway + len) = (char)0xe9;
	*(uintptr_t*)(gateway + len + 1) = jumpAddy;

	if (Hook(src, dst, len)) return gateway;
	return nullptr;
}