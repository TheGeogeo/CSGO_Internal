#include "pch.h"

#include <stdio.h>
#include <tchar.h>

#include "Mem.h"

void Mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess) {
	DWORD oldProtect;
	// VirtualProtectEx permet de definir la protection de la memoire cible d'un autre process 'PAGE_EXECUTE_READWRITE' est obligatoir pour la modification sinon ça crash enfin ça peu
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	//Et la on remet l'ancienne valeur de protection
	VirtualProtectEx(hProcess, dst, size, oldProtect, &oldProtect);
}

void Mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess) {
	BYTE* nopArray = new BYTE[size];
	// Facile a comprendre, le tableau et rempli de la valeur demandé et sont nombre ditération
	memset(nopArray, 0x90, size);

	Mem::PatchEx(dst, nopArray, size, hProcess);
	// delete parce que new créer une valeurr sur le heap du coup faut le delete a la main pour eviter les fuites mémoires
	delete[] nopArray;
}

// DMAA veut dire "Find 'Dynamic Memory Alocation Address'"
uintptr_t Mem::FindDMAAddy(HANDLE hProc, uintptr_t addressPtr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = addressPtr;
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

void Mem::Patch(BYTE* dst, BYTE* src, unsigned int size) {
	DWORD oldProtect;
	// VirtualProtectEx permet de definir la protection de la memoire cible d'un autre process 'PAGE_EXECUTE_READWRITE' est obligatoir pour la modification sinon ça crash enfin ça peu
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dst, src, size);
	//Et la on remet l'ancienne valeur de protection
	VirtualProtect(dst, size, oldProtect, &oldProtect);
}

void Mem::Nop(BYTE* dst, unsigned int size) {
	DWORD oldProtect;
	// VirtualProtectEx permet de definir la protection de la memoire cible d'un autre process 'PAGE_EXECUTE_READWRITE' est obligatoir pour la modification sinon ça crash enfin ça peu
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(dst, 0x90, size);
	//Et la on remet l'ancienne valeur de protection
	VirtualProtect(dst, size, oldProtect, &oldProtect);
}

uintptr_t Mem::FindDMAAddy(uintptr_t addressPtr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = addressPtr;
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}