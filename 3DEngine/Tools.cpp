#include "stdafx.h"
#include "Tools.h"

pt::pt(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void pt::move(float dx, float dy, float dz) {
	x += dx;
	y += dy;
	z += dz;
}

void pt::moveTo(float tx, float ty, float tz) {
	x = tx;
	y = ty;
	z = tz;
}

ed::ed(int start, int end) {
	this->start = start;
	this->end = end;
}

pl::pl() {
	eds = new vector<int>();
	pts = new vector<int>();
}

float dp(pt* a, pt* b) {
	return dp(a->x, a->y, a->z, b->x, b->y, b->z);
}

float dp(float a1, float a2, float a3, float b1, float b2, float b3) {
	return a1 * b1 + a2 * b2 + a3 * b3;
}

float* cp(pt* a, pt* b) {
	return cp(a->x, a->y, a->z, b->x, b->y, b->z);
}

float* cp(float a1, float a2, float a3, float b1, float b2, float b3) {
	float* res = new float[3];

	res[0] = a2 * b3 - a3 * b2;
	res[1] = a3 * b1 - a1 * b3;
	res[2] = a1 * b2 - a2 * b1;

	return res;
}

//m x n * o x p = m x p
float** dot(float** a, int m, int n, float** b, int o, int p) {	
	float** res = new float* [n];
	int i, j, k;

	if (m == p) {
		for (i = 0; i < n; i++) {
			res[i] = new float[o];
			for (j = 0; j < o; j++) {
				res[i][j] = 0;
				for (k = 0; k < m; k++) {
					res[i][j] += a[i][k] * b[k][j];
				}
			}
		}
	}

	return res;
}

void setUpRotationMatrix(float** matrix, float angle, float u, float v, float w)
{
	float L = (u * u + v * v + w * w);	
	angle = angle * (float)3.14159265358979323846 / (float)180.0;
	float u2 = u * u;
	float v2 = v * v;
	float w2 = w * w;

	matrix[0][0] = (u2 + (v2 + w2) * cos(angle)) / L;
	matrix[0][1] = (u * v * (1 - cos(angle)) - w * sqrt(L) * sin(angle)) / L;
	matrix[0][2] = (u * w * (1 - cos(angle)) + v * sqrt(L) * sin(angle)) / L;
	matrix[0][3] = 0.0;

	matrix[1][0] = (u * v * (1 - cos(angle)) + w * sqrt(L) * sin(angle)) / L;
	matrix[1][1] = (v2 + (u2 + w2) * cos(angle)) / L;
	matrix[1][2] = (v * w * (1 - cos(angle)) - u * sqrt(L) * sin(angle)) / L;
	matrix[1][3] = 0.0;

	matrix[2][0] = (u * w * (1 - cos(angle)) - v * sqrt(L) * sin(angle)) / L;
	matrix[2][1] = (v * w * (1 - cos(angle)) + u * sqrt(L) * sin(angle)) / L;
	matrix[2][2] = (w2 + (u2 + v2) * cos(angle)) / L;
	matrix[2][3] = 0.0;

	matrix[3][0] = 0.0;
	matrix[3][1] = 0.0;
	matrix[3][2] = 0.0;
	matrix[3][3] = 1.0;
}

void getRotate(float** rotationMatrix, pt* point, float* res) 
{
	float** inputMatrix = new float* [4];
	inputMatrix[0] = new float[1];
	inputMatrix[0][0] = point->x;
	inputMatrix[1] = new float[1];
	inputMatrix[1][0] = point->y;
	inputMatrix[2] = new float[1];
	inputMatrix[2][0] = point->z;
	inputMatrix[3] = new float[1];
	inputMatrix[3][0] = 1;
	float** outputMatrix = dot(rotationMatrix, 4, 4, inputMatrix, 1, 4);

	res[0] = ceil(outputMatrix[0][0]);
	res[1] = ceil(outputMatrix[1][0]);
	res[2] = ceil(outputMatrix[2][0]);
}