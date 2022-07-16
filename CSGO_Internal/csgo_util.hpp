#pragma once

#include <Windows.h>
#include <thread>

#include "Other.hpp"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

struct BoneMatrix {
	BYTE _junk1[0xC];
	float x;
	BYTE _junk2[0xC];
	float y;
	BYTE _junk3[0xC];
	float z;
};

struct variables
{
	uintptr_t clientDll;
	uintptr_t engineDll;
	uintptr_t* localPlayer;

	uintptr_t nextEnt = 0x10;

	int tbDelay = 0;
	int delayUsageCPU = 5;
	int* iShotFired;

	bool bLowCPU = true;
	bool bUnload = false;
	bool bBhop = false;
	bool bRadar = false;
	bool bAntiFlash = false;
	bool bGlow = false;
	bool bTriggerBot = false;
	bool bAutoRecoil = false;
	bool bAimBot = false;

	vec3* vecOrigin;
	vec3* viewAngles;
	vec3* aimPunchAngle;
}var;

bool isPlayerMoving()
{
	vec3 playerVel = *(vec3*)(*var.localPlayer + m_vecVelocity);
	int vel = playerVel.x + playerVel.y + playerVel.z;
	if (vel != 0)
		return true;
	else
		return false;
}

inline float GetDistance(uintptr_t* entity)
{
	vec3 myLoc = *(vec3*)(*var.localPlayer + m_vecOrigin);
	vec3 entLoc = *(vec3*)(*entity + m_vecOrigin);

	return sqrt(pow(myLoc.x - entLoc.x, 2) + pow(myLoc.y - entLoc.y, 2) + pow(myLoc.z - entLoc.z, 2)) * 0.0254; // get distance between 2 entity
}

inline uintptr_t* GetPlayerByIndex(int index)
{
	return (uintptr_t*)(var.clientDll + dwEntityList + index * var.nextEnt);
}

inline vec3* GetVecOrigin(uintptr_t* entity)
{
	return (vec3*)(*entity + m_vecOrigin);
}

inline vec3* GetViewOffset(uintptr_t* entity)
{
	return (vec3*)(*entity + m_vecViewOffset);
}

inline vec3* GetBonePos(uintptr_t* entity, int boneId)
{
	//uint32_t boneMatrix = *(uint32_t*)(*(uint32_t*)entity + m_dwBoneMatrix);
	uint32_t boneMatrix = *(uint32_t*)(*entity + m_dwBoneMatrix);
	static vec3 bonePos;
	bonePos.x = *(float*)(boneMatrix + 0x30 * boneId + 0x0C);
	bonePos.y = *(float*)(boneMatrix + 0x30 * boneId + 0x1C);
	bonePos.z = *(float*)(boneMatrix + 0x30 * boneId + 0x2C);
	return &bonePos;
}

void InitModule() {
	var.clientDll = (uintptr_t)GetModuleHandle(L"client.dll");
	var.engineDll = (uintptr_t)GetModuleHandle(L"engine.dll");
}

void InitVariable() {
	var.localPlayer = (uintptr_t*)(var.clientDll + dwLocalPlayer);
	if (*var.localPlayer)
	{
		var.iShotFired = (int*)(*var.localPlayer + m_iShotsFired);
		var.vecOrigin = GetVecOrigin(var.localPlayer);
		var.viewAngles = (vec3*)(*(uintptr_t*)(var.engineDll + dwClientState) + dwClientState_ViewAngles);
		var.aimPunchAngle = (vec3*)(*var.localPlayer + m_aimPunchAngle);
	}
}

void UI()
{
	std::system("cls");

	const char* strCPU = var.bLowCPU ? "LOW CPU" : "HIGH CPU";
	const char* strBhop = var.bBhop ? "ON" : "OFF";
	const char* strRadar = var.bRadar ? "ON" : "OFF";
	const char* strAntiFlash = var.bAntiFlash ? "ON" : "OFF";
	const char* strGlow = var.bGlow ? "ON" : "OFF";
	const char* strTrigger = var.bTriggerBot ? "ON" : "OFF";
	const char* strAutoRecoil = var.bAutoRecoil ? "ON" : "OFF";
	const char* strAimbBot = var.bAimBot ? "ON" : "OFF";

	std::cout << "By TheGeogeo" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "NUMPAD1 BunnyHop > " << strBhop << std::endl;
	std::cout << "NUMPAD2 RadarHack > " << strRadar << std::endl;
	std::cout << "NUMPAD3 AntiFlash > " << strAntiFlash << std::endl;
	std::cout << "" << std::endl;
	std::cout << "F1 GlowHack > " << strGlow << std::endl;
	std::cout << "F2 TriggerBot > " << strTrigger << std::endl;
	std::cout << "F3 Auto Recoil > " << strAutoRecoil << std::endl;
	std::cout << "F4 Aim Bot > " << strAimbBot << std::endl;
	std::cout << "" << std::endl;
	std::cout << "INSERT > " << strCPU << " | High cpu is better if you have a good cpu" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "END > Unload Cheat" << std::endl;
}