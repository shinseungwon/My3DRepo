#pragma once
#include "Object.h"
using namespace std;

class Space {
public:
	HWND hWnd;
	COLORREF* board;
	COLORREF** dim;
	DWORD64 size = 0;
	DWORD64 bWidth = 0;
	DWORD64 bHeight = 0;
	BYTE waiting = 10;	

	vector<Object*>* objs;

	float rotationMatrix[4][4];

	COLORREF* colors;

	Space(HWND hWnd, WORD bWidth, WORD bHeight);
	void Clear();
	void Next();
	void Draw();

	void setUpRotationMatrix(float angle, float u, float v, float w);
	void rotate(pt* point);
};

void SpaceStart(Space* space, int w, int h);
void SpaceDraw(Space* space, promise<string>* p);