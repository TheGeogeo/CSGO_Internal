// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "pch.h"
#include <iostream>

#include "csgo.hpp"

#include "Proc.h"
#include "Mem.h"
#include "csgo_util.hpp"
#include "Glow.hpp"
#include "TriggerBot.hpp"
#include "Other.hpp"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

DWORD WINAPI MainHack(HMODULE hModule) {
	AllocConsole();
	FILE* fConsole;
	freopen_s(&fConsole, "CONOUT$", "w", stdout);

	var.clientDll = (DWORD)GetModuleHandle(L"client.dll");
	var.engineDll = (DWORD)GetModuleHandle(L"engine.dll");
	var.localPlayer = var.clientDll + dwLocalPlayer;

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

			if (GetAsyncKeyState(VK_F1) & 1)
			{
				var.bGlow = !var.bGlow;
				UI();

				if (var.bGlow)
				{
					SetBrightness(5.f);
					t.glowT = new std::thread(&HandleGlow);
				}
				else
				{
					CloseThreadCustom(t.glowT, t.bGlowT);
					DisableGlow();
					t.bGlowT = true;
				}
			}

			if (GetAsyncKeyState(VK_F2) & 1)
			{
				var.bTriggerBot = !var.bTriggerBot;
				UI();

				if (var.bTriggerBot)
					t.triggerBotT = new std::thread(&HandleTBot);
				else
				{
					CloseThreadCustom(t.triggerBotT, t.bTriggerBotT);
					t.bTriggerBotT = true;
				}
			}

			if (GetAsyncKeyState(VK_INSERT) & 1)
			{
				var.bLowCPU = !var.bLowCPU;
				UI();
			}

			if (GetAsyncKeyState(VK_END) & 1)
			{
				var.bUnload = true;
				break;
			}

			if (var.bBhop)
			{
				if (isPlayerMoving())
					if (*(int*)Mem::FindDMAAddy(var.localPlayer, { m_fFlags }) == 257 && GetAsyncKeyState(VK_SPACE))
						*(BYTE*)(var.clientDll + dwForceJump) = 6;
			}

			if (var.bRadar)
			{
				for (short i = 0; i < 64; i++)
				{
					DWORD ent = *(DWORD*)(var.clientDll + dwEntityList + i * var.nextEnt);
					if (ent)
					{
						*(BYTE*)(ent + m_bSpotted) = 1;
						*(BYTE*)(ent + m_bSpottedByMask) = 1;
					}
				}
			}

			if (var.bAntiFlash)
			{
				int* flashDur = (int*)Mem::FindDMAAddy((uintptr_t)(var.localPlayer), { m_flFlashDuration });
				if (*flashDur > 0)
				{
					*flashDur = 0;
				}
			}
		}

		if (var.bLowCPU)
			Sleep(5);
	}

	//disable all thing
	CloseAllThreadEndMain();
	DisableGlow();

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