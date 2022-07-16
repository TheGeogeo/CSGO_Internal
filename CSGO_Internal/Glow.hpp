#pragma once

struct ClrRender {
	BYTE red, green, blue;
};

struct GlowStruct
{
	BYTE base[8];
	float red;
	float green;
	float blue;
	float alpha;
	BYTE buffer[16];
	bool renderWhenOccluded;
	bool renderWhenUnOccluded;
	bool fullBloom;
	BYTE buffer1[5];
	int glowStyle;
};

struct state
{
	int health;
	bool defusing;
	ClrRender clrRender;
};

void SetBrightness(float bright, bool bXor = true) {
	int ptr = *(int*)(var.engineDll + model_ambient_min);
	int xorPtr;
	if (bXor)
		xorPtr = *(int*)&bright ^ ptr;
	else
		xorPtr = *(int*)&bright;
	*(int*)(var.engineDll + model_ambient_min) = xorPtr;
}

inline void SetTeamGlow(uintptr_t entity, GlowStruct& gt, state& st) {
	gt.red = 0.f;
	gt.green = 0.f;
	gt.blue = 2.f;
	gt.alpha = 1.f;
	gt.renderWhenOccluded = true;
	gt.renderWhenUnOccluded = false;
	gt.fullBloom = false;

	st.clrRender.red = 0;
	st.clrRender.green = 0;
	st.clrRender.blue = 255;
}

inline void SetEnemyGlow(uintptr_t entity, GlowStruct& gt, state& st) {
	if (st.defusing)
	{
		gt.red = 1.f;
		gt.green = 1.f;
		gt.blue = 0.5f;
		gt.alpha = 1.f;
	}
	else
	{
		gt.red = st.health * -0.01f + 1.f;
		gt.green = st.health * 0.01f;
		gt.blue = 0.f;
		gt.alpha = 1.f;
	}
	gt.renderWhenOccluded = true;
	gt.renderWhenUnOccluded = false;
	gt.fullBloom = false;

	st.clrRender.red = 0;
	st.clrRender.green = 255;
	st.clrRender.blue = 0;
}

inline void HandleGlow()
{
	SetBrightness(5.f);

	while (t.bGlowT)
	{
		if (var.bLowCPU)
			Sleep(var.delayUsageCPU);

		if (!*var.localPlayer) continue;

		uintptr_t glowObject = *(DWORD*)(var.clientDll + dwGlowObjectManager);
		int myTeam = *(int*)(*var.localPlayer + m_iTeamNum);

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

	SetBrightness(-5.f);
	SetBrightness(5.f);
	SetBrightness(-5.f);

	ClrRender clrRender;
	clrRender.red = -1;
	clrRender.green = -1;
	clrRender.blue = -1;

	for (short int i = 0; i < 64; i++)
	{
		uintptr_t* entity = (uintptr_t*)(var.clientDll + dwEntityList + i * var.nextEnt);
		if (*entity)
		{
			*(ClrRender*)(*entity + m_clrRender) = clrRender;
		}
	}
}