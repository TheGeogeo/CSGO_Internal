#pragma once

inline uintptr_t* GetClosestEnemy()
{
	float closestDist = 1000000.f;
	int closestDistIndex = -1;

	int myTeam = *(int*)(*var.localPlayer + m_iTeamNum);
	int myHealth = *(int*)(*var.localPlayer + m_iHealth);

	for (int i = 0; i < 64; i++)
	{
		uintptr_t* entity = (uintptr_t*)(var.clientDll + dwEntityList + i * var.nextEnt);
		if (*entity && *var.localPlayer != *entity)
		{
			int team = *(int*)(*entity + m_iTeamNum);
			int health = *(int*)(*entity + m_iHealth);
			if (myTeam != team && myHealth > 0 && health > 0)
			{
				float currentDist = GetDistance(entity);
				if (currentDist < closestDist)
				{
					closestDist = currentDist;
					closestDistIndex = i;
				}
			}
		}
	}
	return closestDistIndex == -1 ? nullptr : GetPlayerByIndex(closestDistIndex);
}

inline void AimAt(vec3* target)
{
	double pi = 3.14159265358979323846;

	vec3 origin = *GetVecOrigin(var.localPlayer);
	vec3 viewOffset = *GetViewOffset(var.localPlayer);
	vec3* myPos = &(origin % viewOffset); // is not a modulo look vec3 struct

	vec3 deltaVec = { target->x - myPos->x, target->y - myPos->y, target->z - myPos->z };
	float deltaVecLength = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);

	float pitch = -asin(deltaVec.z / deltaVecLength) * (180 / pi);
	float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / pi);

	// dodge instant ban without break the range angles
	if (pitch >= -89.f && pitch <= 89.f && yaw >= -180.f && yaw <= 180.f)
	{
		var.viewAngles->x = pitch;
		var.viewAngles->y = yaw;
	}
}

inline void MainAimBot() {
	while (t.bAimBotT)
	{
		if (var.bLowCPU)
			Sleep(var.delayUsageCPU);

		if (!*var.localPlayer) continue;

		uintptr_t* closestEnemy = GetClosestEnemy();
		if (closestEnemy)
		{
			AimAt(GetBonePos(closestEnemy, 8));
		}
	}
}