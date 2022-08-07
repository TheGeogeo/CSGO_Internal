#include "Includes.h"

extern Hack* hack;

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR col) {
	D3DRECT rect = { x,y,x + w,y + h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, col, 0, 0);
}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR col) {
	if (!hack->LineL)
		D3DXCreateLine(pDevice, &hack->LineL);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	hack->LineL->SetWidth(thickness);
	hack->LineL->Draw(Line, 2, col);
}

void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR col) {
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, col);
}

void DrawEspBox2D(Vec2 t, Vec2 b, int thickness, D3DCOLOR col) {
	int height = ABS(t.y - b.y);
	Vec2 tl, tr;
	tl.x = t.x - height / 4;
	tr.x = t.x + height / 4;
	tl.y = tr.y = t.y;
	Vec2 bl, br;
	bl.x = b.x - height / 4;
	br.x = b.x + height / 4;
	bl.y = br.y = b.y;

	DrawLine(tl, tr, thickness, col);
	DrawLine(bl, br, thickness, col);
	DrawLine(tl, bl, thickness, col);
	DrawLine(tr, br, thickness, col);
}