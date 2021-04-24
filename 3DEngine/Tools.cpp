#include "stdafx.h"
#include "Tools.h"

pt::pt(int x, int y, int z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

ed::ed(int start, int end) {
	this->start = start;
	this->end = end;
}

pl::pl() {
	eds = new vector<int>();
	pts = new vector<int>();
}