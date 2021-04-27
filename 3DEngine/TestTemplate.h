#pragma once
using namespace std;
#include "Tools.h"
#include "Space.h"

class TestTemplate : public Space {
public:
	float** pivotMatrix;
	float** rotationMatrix;
	TestTemplate(HWND hWnd, WORD bWidth, WORD bHeight);
	void Next();
};