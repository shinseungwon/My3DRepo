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
	float** rotationMatrix;

	Space(HWND hWnd, WORD bWidth, WORD bHeight);
	void Clear();	
	void Draw();
	virtual void Next();
};

void SpaceStart(Space* space, int w, int h);
void SpaceDraw(Space* space, promise<string>* p);