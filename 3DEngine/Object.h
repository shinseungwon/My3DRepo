#pragma once
using namespace std;
#include "Tools.h"

class Object {
public:
	float cx = 0;
	float cy = 0;
	float cz = 0;

	vector<pt*>* points;
	vector<ed*>* edges;
	vector<pl*>* planes;

	Object();
	void Center();
	void NVec(pl* plane);
};