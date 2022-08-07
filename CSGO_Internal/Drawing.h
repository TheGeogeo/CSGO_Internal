#pragma once

#include "Hack.h"

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR col);

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR col);
void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR col);

void DrawEspBox2D(Vec2 t, Vec2 b, int thickness, D3DCOLOR col);