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

inline void GetDistance(DWORD entity)
{
	vec3 myLoc = *(vec3*)(var.localPlayer + m_vecOrigin);
	vec3 entLoc = *(vec3*)Mem::FindDMAAddy(entity, { m_vecOrigin });

	float dist = sqrt(pow(myLoc.x - entLoc.x, 2) + pow(myLoc.y - entLoc.y, 2) + pow(myLoc.z - entLoc.z, 2)) * 0.0254; // get distance between 2 entity
	var.tbDelay = dist * 3.3;
}

inline bool CheckTBot()
{
	int crossHairId = *(int*)(var.localPlayer + m_iCrosshairId);
	if (crossHairId != 0 && crossHairId < 64)
	{
		int myTeam = *(int*)(var.localPlayer + m_iTeamNum);

		DWORD entity = var.clientDll + dwEntityList + ((crossHairId - 1) * var.nextEnt);
		int team = *(int*)Mem::FindDMAAddy(entity, { m_iTeamNum });
		int health = *(int*)Mem::FindDMAAddy(entity, { m_iHealth });

		if (myTeam != team && health > 0)
		{
			GetDistance(entity);
			return true;
		}
	}

	return false;
}

inline void HandleTBot()
{
	while (t.bTriggerBotT)
	{
		if (CheckTBot())
			Shoot();

		if (var.bLowCPU)
			Sleep(var.delayUsageCPU);
	}
}