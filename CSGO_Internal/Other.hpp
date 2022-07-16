#pragma once

#include <thread>

struct vec3
{
	float x, y, z;

	vec3& operator%(vec3 arg) // dodge conflit i hopre i never use true modulo on vec3 lmao
	{
		x += arg.x;
		y += arg.y;
		z += arg.z;
		return *this;
	}

	vec3 operator+(vec3 d) {
		return { x + d.x, y + d.y, z + d.z };
	}
	vec3 operator-(vec3 d) {
		return { x - d.x, y - d.y, z - d.z };
	}
	vec3 operator*(float d) {
		return { x * d, y * d,z * d };
	}

	void Normalize() {
		while (y < -180) y += 360;
		while (y > 180) y -= 360;
		if (x > 89) y = 89;
		if (x < -89) y = -89;
	}
};

struct thread
{
	// if you use c++ std under 17, asign 'atomic' with >> ATOMIC_VAR_INIT(x)
	std::atomic_bool bGlowT = true;
	std::thread* glowT = nullptr;

	std::atomic_bool bTriggerBotT = true;
	std::thread* triggerBotT = nullptr;

	std::atomic_bool bAutoRecoilT = true;
	std::thread* autoRecoilT = nullptr;

	std::atomic_bool bAimBotT = true;
	std::thread* aimBotT = nullptr;
}t;

inline void CloseThreadCustom(std::thread* th, std::atomic_bool& bT)
{
	bT = false;
	bool joinned = false;
	while (!joinned)
	{
		if (th->joinable())
		{
			th->join();
			joinned = true;
		}
	}
	delete th;
	th = nullptr;
	bT = true;
}

inline void CloseAllThreadEndMain()
{
	if (t.glowT && t.glowT != nullptr)
		CloseThreadCustom(t.glowT, t.bGlowT);

	if (t.triggerBotT && t.triggerBotT != nullptr)
		CloseThreadCustom(t.triggerBotT, t.bTriggerBotT);

	if (t.bAutoRecoilT && t.autoRecoilT != nullptr)
		CloseThreadCustom(t.autoRecoilT, t.bAutoRecoilT);

	if (t.bAimBotT && t.aimBotT != nullptr)
		CloseThreadCustom(t.aimBotT, t.bAimBotT);
}