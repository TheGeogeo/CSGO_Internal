#pragma once

#include <thread>

struct vec3
{
	float x, y, z;
};

struct thread
{
	// if you use c++ std under 17, asign 'atomic' with >> ATOMIC_VAR_INIT(x)
	std::atomic_bool bGlowT = true;
	std::thread* glowT = nullptr;

	std::atomic_bool bTriggerBotT = true;
	std::thread* triggerBotT = nullptr;
}t;

void CloseThreadCustom(std::thread* th, std::atomic_bool& bT)
{
	bT = false;
	th->join();
	delete th;
	th = nullptr;
}

void CloseAllThreadEndMain()
{
	if (t.glowT)
		CloseThreadCustom(t.glowT, t.bGlowT);

	if (t.triggerBotT)
		CloseThreadCustom(t.triggerBotT, t.bTriggerBotT);
}