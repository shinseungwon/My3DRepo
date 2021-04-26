#pragma once
using namespace std;
#include "Tools.h"
#include "Space.h"

class TestTemplate : public Space {
public:
	TestTemplate(HWND hWnd, WORD bWidth, WORD bHeight);
	void Next();
};