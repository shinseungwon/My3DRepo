#pragma once
using namespace std;

class pt {
public:
	float x;
	float y;
	float z;	
	pt(int x, int y, int z);
};

class ed {
public:
	int start;
	int end;
	ed(int start, int end);
};

class pl {
public:
	float nVecX = 0;
	float nVecY = 0;
	float nVecZ = 0;

	vector<int>* eds;
	vector<int>* pts;
	pl();	
};