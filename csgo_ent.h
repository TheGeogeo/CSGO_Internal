#pragma once
#include <Windows.h>

struct offsets
{
	DWORD entityList = 0x4DDB91C;
	DWORD nextEnt = 0x10;
	DWORD dwLocalPlayer = 0xDBF4CC;

	DWORD m_iHealth = 0x100;
	DWORD m_iArmor = 0x117CC;
	DWORD m_iTeam = 0xf4;
	DWORD m_bSpottedByMask = 0x93d;
	DWORD m_bSpotted = 0x980;
	DWORD m_iFlashDuration = 0x10470;
	DWORD m_iObserverMode = 0x3388;
	DWORD m_fFlags = 0x104;
	DWORD m_vecVelocity = 0x114;

	DWORD dwGlowObjectManager = 0x5324588;
	DWORD dwForceJump = 0x52858A8;
	DWORD m_iGlowIndex = 0x10488;
}offset;

struct variables
{
	DWORD clientDll;
	DWORD localPlayer;

	bool bBhop = false;
	bool bRadar = false;
	bool bAntiFlash = false;
	bool bGlow = false;
}var;

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

struct vec3
{
	float x, y, z;
};

bool isPlayerMoving()
{
	vec3 playerVel = *(vec3*)Mem::FindDMAAddy(var.localPlayer, { offset.m_vecVelocity });
	int vel = playerVel.x + playerVel.y + playerVel.z;
	if (vel != 0)
		return true;
	else
		return false;
}