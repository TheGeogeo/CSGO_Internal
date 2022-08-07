#pragma once

#include "csgo.hpp"
using namespace hazedumper;

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#define ABS(x) ((x < 0) ? (-x) : (x))

struct Vec2 {
	float x, y;
};

struct Vec3 {
	float x, y, z;
};

struct Vec4 {
	float x, y, z, w;
};

class Ent
{
public:

	union
	{
		DEFINE_MEMBER_N(bool, m_bDormant, signatures::m_bDormant);
		DEFINE_MEMBER_N(int, m_iHealth, netvars::m_iHealth);
		DEFINE_MEMBER_N(Vec3, m_vecOrigin, netvars::m_vecOrigin);
		DEFINE_MEMBER_N(int, m_iTeamNum, netvars::m_iTeamNum);
		DEFINE_MEMBER_N(int, m_dwBoneMatrix, netvars::m_dwBoneMatrix);
		DEFINE_MEMBER_N(int, m_ArmorValue, netvars::m_ArmorValue);
		DEFINE_MEMBER_N(Vec3, m_aimPunchAngle, netvars::m_aimPunchAngle);
	};
};

class EntListObj
{
public:

	struct Ent* ent;
	char padding[12];
};

class EntList
{
public:

	EntListObj ents[32];
};

class Hack
{
public:
	uintptr_t dwEntityList = signatures::dwEntityList;
	uintptr_t dwViewMatrix = signatures::dwViewMatrix;

	uintptr_t engine;
	uintptr_t client;
	Ent* localEnt;
	EntList* entList;
	float viewMatrix[16];

	ID3DXLine* LineL;

	Vec2 crosshair2D;
	int crosshairSize = 4;

	void Init();
	void Update();
	bool CheckValidEnt(Ent* ent);
	bool WorldToScreen(Vec3 pos, Vec2& screen);
	Vec3 GetbonePos(Ent* ent, int bone);
};