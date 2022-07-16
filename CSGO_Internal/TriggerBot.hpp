#pragma once

#include <cmath>

#include "Other.hpp"

inline void Shoot()
{
	Sleep(var.tbDelay);
	*(int*)(var.clientDll + dwForceAttack) = 5;
	Sleep(20);
	*(int*)(var.clientDll + dwForceAttack) = 4;
}

inline bool CheckTBot()
{
	int crossHairId = *(int*)(*var.localPlayer + m_iCrosshairId);
	if (crossHairId != 0 && crossHairId < 64)
	{
		int myTeam = *(int*)(*var.localPlayer + m_iTeamNum);

		uintptr_t* entity = (uintptr_t*)(var.clientDll + dwEntityList + ((crossHairId - 1) * var.nextEnt));
		int team = *(int*)(*entity + m_iTeamNum);
		int health = *(int*)(*entity + m_iHealth);

		if (myTeam != team && health > 0)
		{
			float dist = GetDistance(entity);
			var.tbDelay = dist * 3.3;
			return true;
		}
	}

	return false;
}

inline void HandleTBot()
{
	while (t.bTriggerBotT)
	{
		if (var.bLowCPU)
			Sleep(var.delayUsageCPU);

		if (!*var.localPlayer) continue;

		if (CheckTBot())
			Shoot();
	}
}