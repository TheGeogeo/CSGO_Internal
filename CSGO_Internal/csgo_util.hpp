#pragma once

#include <Windows.h>
#include <thread>

#include "Other.hpp"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

struct variables
{
	uintptr_t clientDll;
	uintptr_t engineDll;
	uintptr_t localPlayer;

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

	vec3* viewAngles;
	vec3* aimPunchAngle;
}var;

bool isPlayerMoving()
{
	vec3 playerVel = *(vec3*)(var.localPlayer + m_vecVelocity);
	int vel = playerVel.x + playerVel.y + playerVel.z;
	if (vel != 0)
		return true;
	else
		return false;
}

void InitModule() {
	var.clientDll = (uintptr_t)GetModuleHandle(L"client.dll");
	var.engineDll = (uintptr_t)GetModuleHandle(L"engine.dll");
}

void InitVariable() {
	var.localPlayer = *(uintptr_t*)(var.clientDll + dwLocalPlayer);
	var.iShotFired = (int*)(var.localPlayer + m_iShotsFired);
	var.viewAngles = (vec3*)(*(uintptr_t*)(var.engineDll + dwClientState) + dwClientState_ViewAngles);
	var.aimPunchAngle = (vec3*)(var.localPlayer + m_aimPunchAngle);
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

	std::cout << "By TheGeogeo" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "NUMPAD1 BunnyHop > " << strBhop << std::endl;
	std::cout << "NUMPAD2 RadarHack > " << strRadar << std::endl;
	std::cout << "NUMPAD3 AntiFlash > " << strAntiFlash << std::endl;
	std::cout << "" << std::endl;
	std::cout << "F1 GlowHack > " << strGlow << std::endl;
	std::cout << "F2 TriggerBot > " << strTrigger << std::endl;
	std::cout << "F3 Auto Recoil > " << strAutoRecoil << std::endl;
	std::cout << "" << std::endl;
	std::cout << "INSERT > " << strCPU << " | High cpu is better if you have a good cpu" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "END > Unload Cheat" << std::endl;
}