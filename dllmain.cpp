// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "pch.h"
#include <iostream>

#include "Proc.h"
#include "Mem.h"
#include "csgo_ent.h"

void UI() {
	std::system("cls");

	const char* strBhop = var.bBhop ? "ON" : "OFF";
	const char* strRadar = var.bRadar ? "ON" : "OFF";
	const char* strAntiFlash = var.bAntiFlash ? "ON" : "OFF";
	const char* strGlow = var.bGlow ? "ON" : "OFF";

	std::cout << "By TheGeogeo" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "NUMPAD1 BunnyHop > " << strBhop << std::endl;
	std::cout << "NUMPAD2 RadarHack > " << strRadar << std::endl;
	std::cout << "NUMPAD3 AntiFlash > " << strAntiFlash << std::endl;
	std::cout << "" << std::endl;
	std::cout << "NUMPAD0 GlowHack > " << strGlow << std::endl;
	std::cout << "" << std::endl;
	std::cout << "END > Unload Cheat" << std::endl;
}

DWORD WINAPI MainHack(HMODULE hModule) {
	AllocConsole();
	FILE* fConsole;
	freopen_s(&fConsole, "CONOUT$", "w", stdout);

	var.clientDll = (DWORD)GetModuleHandle(L"client.dll");
	var.localPlayer = var.clientDll + offset.dwLocalPlayer;

	UI();

	while (true)
	{
		if (var.clientDll && *(DWORD*)var.localPlayer)
		{
			if (GetAsyncKeyState(VK_NUMPAD1) & 1)
			{
				var.bBhop = !var.bBhop;
				UI();
			}

			if (GetAsyncKeyState(VK_NUMPAD2) & 1)
			{
				var.bRadar = !var.bRadar;
				UI();
			}

			if (GetAsyncKeyState(VK_NUMPAD3) & 1)
			{
				var.bAntiFlash = !var.bAntiFlash;
				UI();
			}

			if (GetAsyncKeyState(VK_NUMPAD0) & 1)
			{
				var.bGlow = !var.bGlow;
				UI();
			}

			if (GetAsyncKeyState(VK_END) & 1)
			{
				break;
			}

			if (var.bBhop)
			{
				if (isPlayerMoving())
					if (*(int*)Mem::FindDMAAddy(var.localPlayer, { offset.m_fFlags }) == 257 && GetAsyncKeyState(VK_SPACE))
						*(BYTE*)(var.clientDll + offset.dwForceJump) = 6;
			}

			if (var.bRadar)
			{
				for (short i = 0; i < 64; i++)
				{
					DWORD ent = *(DWORD*)(var.clientDll + offset.entityList + i * offset.nextEnt);
					if (ent)
					{
						*(BYTE*)(ent + offset.m_bSpotted) = 1;
						*(BYTE*)(ent + offset.m_bSpottedByMask) = 1;
					}
				}
			}

			if (var.bAntiFlash)
			{
				int* flashDur = (int*)Mem::FindDMAAddy((uintptr_t)(var.localPlayer), { offset.m_iFlashDuration });
				if (*flashDur > 0)
				{
					*flashDur = 0;
				}
			}

			if (var.bGlow)
			{
				DWORD glowObject = *(DWORD*)(var.clientDll + offset.dwGlowObjectManager);
				int myTeam = *(int*)Mem::FindDMAAddy((uintptr_t)var.localPlayer, { offset.m_iTeam });

				for (short int i = 0; i < 64; i++)
				{
					DWORD entity = var.clientDll + offset.entityList + i * offset.nextEnt;
					if (*(DWORD*)entity)
					{
						int team = *(int*)Mem::FindDMAAddy((uintptr_t)entity, { offset.m_iTeam });
						int glowIndex = *(int*)Mem::FindDMAAddy((uintptr_t)entity, { offset.m_iGlowIndex });

						if (team != myTeam)
						{
							if (true)
							{
							}
							else {
								*(float*)(glowObject + ((glowIndex * 0x38) + 0x8)) = 2.f; //red
								*(float*)(glowObject + ((glowIndex * 0x38) + 0xc)) = 0.f; //green
								*(float*)(glowObject + ((glowIndex * 0x38) + 0x10)) = 0.f; //blue
								*(float*)(glowObject + ((glowIndex * 0x38) + 0x14)) = 1.7f; //alpha
							}
						}
						else {
							*(float*)(glowObject + ((glowIndex * 0x38) + 0x8)) = 0.f;
							*(float*)(glowObject + ((glowIndex * 0x38) + 0xc)) = 0.f;
							*(float*)(glowObject + ((glowIndex * 0x38) + 0x10)) = 2.f;
							*(float*)(glowObject + ((glowIndex * 0x38) + 0x14)) = 1.7f;
						}

						*(bool*)(glowObject + ((glowIndex * 0x38) + 0x28)) = true; //occluded
						*(bool*)(glowObject + ((glowIndex * 0x38) + 0x29)) = false; //unOcluded
					}
				}
			}
		}
		Sleep(1);
	}

	int tmpaa = fclose(fConsole);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE hThread = nullptr;
		hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainHack, hModule, NULL, NULL);
		if (hThread) {
			CloseHandle(hThread);
		}
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}