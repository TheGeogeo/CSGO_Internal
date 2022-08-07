#pragma once

#include "Includes.h"

void* d3d9Device[119];
BYTE endSceneByte[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;
Hack* hack;

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice)
		pDevice = o_pDevice;

	//draw square
	//DrawFilledRect(25, 25, 100, 100, RED);

	for (int i = 1; i < 32; i++)
	{
		Ent* curEnt = hack->entList->ents[i].ent;
		if (!hack->CheckValidEnt(curEnt))
			continue;

		D3DCOLOR colTeam;
		if (curEnt->m_iTeamNum == hack->localEnt->m_iTeamNum)
			colTeam = GREEN;
		else
			colTeam = RED;

		Vec3 entHead3D = hack->GetbonePos(curEnt, 8);
		entHead3D.z += 8; // box upper than head
		Vec2 entPos2D, entHead2D;
		if (hack->WorldToScreen(curEnt->m_vecOrigin, entPos2D))
		{
			if (var.bSnapLine)
				DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, colTeam);

			if (hack->WorldToScreen(entHead3D, entHead2D))
			{
				DrawEspBox2D(entPos2D, entHead2D, 1, colTeam);

				int height = ABS(entPos2D.y - entHead2D.y);
				int dx = (entPos2D.x - entHead2D.x);

				float healthPerc = curEnt->m_iHealth / 100.f;
				float armorPerc = curEnt->m_ArmorValue / 100.f;

				Vec2 botHealth, topHealth, botArmor, topArmor;
				int healthHeight = height * healthPerc;
				int armorHeight = height * armorPerc;

				botHealth.y = botArmor.y = entPos2D.y;

				botHealth.x = entPos2D.x - (height / 4) - 2;
				botArmor.x = entPos2D.x + (height / 4) + 2;

				topHealth.y = entHead2D.y + height - healthHeight;
				topArmor.y = entHead2D.y + height - armorHeight;

				topHealth.x = entPos2D.x - (height / 4) - 2 - (dx * healthPerc);
				topArmor.x = entPos2D.x + (height / 4) + 2 - (dx * armorPerc);

				DrawLine(botHealth, topHealth, 4, GREEN);
				DrawLine(botArmor, topArmor, 4, BLUE);
			}
		}
	}

	oEndScene(pDevice);
}

inline void MainEsp2D() {
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(endSceneByte, (char*)d3d9Device[42], 7);

		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
	}

	hack = new Hack();
	hack->Init();

	while (t.bEsp2DT)
	{
		if (var.bLowCPU)
			Sleep(var.delayUsageCPU);

		if (!*var.localPlayer) continue;

		if (GetAsyncKeyState(VK_F6) & 1)
		{
			var.bSnapLine = !var.bSnapLine;
			UI();
		}

		hack->Update();
	}

	var.bSnapLine = false;

	Patch((BYTE*)d3d9Device[42], endSceneByte, 7);
}