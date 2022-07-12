#pragma once

#include <windows.h>
#include <TlHelp32.h>
#include <iostream>

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);