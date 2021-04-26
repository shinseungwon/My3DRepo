#pragma once
using namespace std;
#include "Tools.h"

class Object {
public:

	vector<pt*>* points;
	vector<ed*>* edges;
	vector<pl*>* planes;

	Object();	
	void Center(float* res); //save center xyz
	void NVec(pl* plane);
	void Move(float dx, float dy, float dz);
	void Rotate(float** pivotMatrix);
	void Pivot(float** pivotMatrix);
};
