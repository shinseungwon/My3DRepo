#pragma once
using namespace std;
#include "Tools.h"

class Object {
public:
	//vector<tuple<int, int, int>>* points; //x, y
	//vector<pair<int, int>>* edges; //point seq1 to seq2

	vector<pt*>* points;
	vector<ed*>* edges;

	Object();
};