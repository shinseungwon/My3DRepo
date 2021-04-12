#pragma once
#include "Object.h"
using namespace std;

class Space {
public:
	HWND hWnd;
	COLORREF* board;
	DWORD64 bWidth = 0;
	DWORD64 bHeight = 0;
	BYTE waiting = 10;

	//BYTE** frame;

	vector<Object*>* objs;

	Space(HWND hWnd, WORD bWidth, WORD bHeight);
	void Clear();
	void Next();
	void Draw();
};

void SpaceStart(Space* space, int w, int h);
void SpaceDraw(Space* space, promise<string>* p);

void DrawLine(Space* space, int x1, int y1, int x2, int y2);