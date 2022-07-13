// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "pch.h"
#include <iostream>

#include "csgo.hpp"

#include "Proc.h"
#include "Mem.h"
#include "csgo_util.h"
#include "Glow.h"

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

			if (GetAsyncKeyState(VK_NUMPAD0) & 1)
			{
				var.bGlow = !var.bGlow;
				UI();

				if (var.bGlow)
					SetBrightness(5.f);
				else
					DisableGlow();
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

			if (var.bGlow)
			{
				uintptr_t glowObject = *(DWORD*)(var.clientDll + dwGlowObjectManager);
				int myTeam = *(int*)Mem::FindDMAAddy((uintptr_t)var.localPlayer, { m_iTeamNum });

				for (short int i = 0; i < 64; i++)
				{
					uintptr_t entity = var.clientDll + dwEntityList + i * var.nextEnt;
					if (*(uintptr_t*)entity)
					{
						int team = *(int*)Mem::FindDMAAddy(entity, { m_iTeamNum });
						int glowIndex = *(int*)Mem::FindDMAAddy(entity, { m_iGlowIndex });
						state st;
						st.health = *(int*)Mem::FindDMAAddy(entity, { m_iHealth });
						st.defusing = *(bool*)Mem::FindDMAAddy(entity, { m_bIsDefusing });

						if (team != myTeam)
						{
							GlowStruct Tgt = *(GlowStruct*)(glowObject + (glowIndex * 0x38));
							SetEnemyGlow(entity, Tgt, st);
							*(GlowStruct*)(glowObject + (glowIndex * 0x38)) = Tgt;
							*(ClrRender*)Mem::FindDMAAddy(entity, { m_clrRender }) = st.clrRender;
						}
						else {
							GlowStruct Egt = *(GlowStruct*)(glowObject + (glowIndex * 0x38));
							SetTeamGlow(entity, Egt, st);
							*(GlowStruct*)(glowObject + (glowIndex * 0x38)) = Egt;
							*(ClrRender*)Mem::FindDMAAddy(entity, { m_clrRender }) = st.clrRender;
						}
					}
				}
			}
		}
	}

	//disable all thing
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