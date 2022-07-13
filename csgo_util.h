#pragma once
#include <Windows.h>

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

struct variables
{
	DWORD clientDll;
	DWORD engineDll;
	DWORD localPlayer;

	DWORD nextEnt = 0x10;

	bool bUnload = false;

	bool bBhop = false;
	bool bRadar = false;
	bool bAntiFlash = false;
	bool bGlow = false;
}var;

struct vec3
{
	float x, y, z;
};

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

	const char* strBhop = var.bBhop ? "ON" : "OFF";
	const char* strRadar = var.bRadar ? "ON" : "OFF";
	const char* strAntiFlash = var.bAntiFlash ? "ON" : "OFF";
	const char* strGlow = var.bGlow ? "ON" : "OFF";

	std::cout << "By TheGeogeo" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "NUMPAD1 BunnyHop > " << strBhop << std::endl;
	std::cout << "NUMPAD2 RadarHack > " << strRadar << std::endl;
	std::cout << "NUMPAD3 AntiFlash > " << strAntiFlash << std::endl;
	std::cout << "" << std::endl;
	std::cout << "NUMPAD0 GlowHack > " << strGlow << std::endl;
	std::cout << "" << std::endl;
	std::cout << "END > Unload Cheat" << std::endl;
}