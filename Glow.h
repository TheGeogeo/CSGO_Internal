#pragma once

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
};

inline void SetTeamGlow(uintptr_t entity, GlowStruct& gt) {
	gt.red = 0.f;
	gt.green = 0.f;
	gt.blue = 2.f;
	gt.alpha = 1.f;
	gt.renderWhenOccluded = true;
	gt.renderWhenUnOccluded = false;
	gt.fullBloom = false;
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
}