#pragma once

#include "Other.hpp"

inline void HandleAutoRecoil() {
	vec3 oPunch = { 0,0,0 };
	while (t.bAutoRecoilT)
	{
		if (var.bLowCPU)
			Sleep(var.delayUsageCPU);

		if (!*var.localPlayer) continue;

		vec3 punchAngle = *var.aimPunchAngle * 2;
		if (*var.iShotFired > 1)
		{
			// calc recoil control
			vec3 newAngle = *var.viewAngles + oPunch - punchAngle;

			newAngle.Normalize();

			// set
			*var.viewAngles = newAngle;
		}
		//fix
		oPunch = punchAngle;
	}
}