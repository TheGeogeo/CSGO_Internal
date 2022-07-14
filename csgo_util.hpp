#pragma once

#include <Windows.h>
#include <thread>

#include "Other.hpp"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

struct variables
{
	DWORD clientDll;
	DWORD engineDll;
	DWORD localPlayer;

	DWORD nextEnt = 0x10;

	int tbDelay = 0;
	int delayUsageCPU = 0;

	bool bLowCPU = true;
	bool bUnload = false;
	bool bBhop = false;
	bool bRadar = false;
	bool bAntiFlash = false;
	bool bGlow = false;
	bool bTriggerBot = false;
}var;

bool isPlayerMoving()
{
	vec3 playerVel = *(vec3*)Mem::FindDMAAddy(var.localPlayer, { m_vecVelocity });
	int vel = playerVel.x + playerVel.y + playerVel.z;
	if (vel != 0)
		return true;
	else
		return false;
}

void UI() {
	std::system("cls");

	const char* strCPU = var.bLowCPU ? "LOW CPU" : "HIGH CPU";
	const char* strBhop = var.bBhop ? "ON" : "OFF";
	const char* strRadar = var.bRadar ? "ON" : "OFF";
	const char* strAntiFlash = var.bAntiFlash ? "ON" : "OFF";
	const char* strGlow = var.bGlow ? "ON" : "OFF";
	const char* strTrigger = var.bTriggerBot ? "ON" : "OFF";

	std::cout << "By TheGeogeo" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "NUMPAD1 BunnyHop > " << strBhop << std::endl;
	std::cout << "NUMPAD2 RadarHack > " << strRadar << std::endl;
	std::cout << "NUMPAD3 AntiFlash > " << strAntiFlash << std::endl;
	std::cout << "" << std::endl;
	std::cout << "F1 GlowHack > " << strGlow << std::endl;
	std::cout << "F2 TriggerBot > " << strTrigger << std::endl;
	std::cout << "" << std::endl;
	std::cout << "INSERT > " << strCPU << " | High cpu is better if you have a good cpu" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "END > Unload Cheat" << std::endl;
}