#include "stdafx.h"
#include "Object.h"

Object::Object() {
	points = new vector<pt*>();
	edges = new vector<ed*>();
}