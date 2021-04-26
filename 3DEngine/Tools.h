#pragma once
using namespace std;

class pt {
public:
	float x;
	float y;
	float z;

	pt(float x, float y, float z);
	void move(float dx, float dy, float dz);
	void moveTo(float tx, float ty, float tz);
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

	COLORREF color;

	vector<int>* eds;
	vector<int>* pts;
	pl();	
};

float dp(pt* a, pt* b);
float dp(float a1, float a2, float a3, float b1, float b2, float b3);

float* cp(pt* a, pt* b);
float* cp(float a1, float a2, float a3, float b1, float b2, float b3);

float** dot(float** a, int m, int n, float** b, int o, int p);

void setUpRotationMatrix(float** matrix, float angle, float u, float v, float w);

void getRotate(float** rotationMatrix, pt* point, float* res);