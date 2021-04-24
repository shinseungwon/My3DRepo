#include "stdafx.h"
#include "Object.h"

Object::Object() {
	points = new vector<pt*>();
	edges = new vector<ed*>();
	planes = new vector<pl*>();
}

void Object::Center() {
	cx = 0;
	cy = 0;
	cz = 0;
	for (int i = 0; i < points->size(); i++) {
		cx += points->at(i)->x;
		cy += points->at(i)->y;
		cz += points->at(i)->z;
	}
	cx /= points->size();
	cy /= points->size();
	cz /= points->size();
}


void Object::NVec(pl* plane) {
	pt* p1 = points->at(plane->pts->at(0) - 1);
	pt* p2 = points->at(plane->pts->at(1) - 1);
	pt* p3 = points->at(plane->pts->at(2) - 1);	

	//object center : cx cy cz

	//plane center
	float pcx = (p1->x + p2->x + p3->x) / 3;
	float pcy = (p1->y + p2->y + p3->y) / 3;
	float pcz = (p1->z + p2->z + p3->z) / 3;	

	//object center -> plane center vector
	float octopcx = pcx - cx;
	float octopcy = pcy - cy;
	float octopcz = pcz - cz;

	float p12x = p2->x - p1->x; //a1
	float p12y = p2->y - p1->y; //a2
	float p12z = p2->z - p1->z; //a3
	float p13x = p3->x - p1->x; //b1
	float p13y = p3->y - p1->y; //b2
	float p13z = p3->z - p1->z; //b3

	float nvecx = p12y * p13z - p12z * p13y; //a2*b3 - a3*b2
	float nvecy = p12z * p13x - p12x * p13z; //a3*b1 - a1*b3
	float nvecz = p12x * p13y - p12y * p13x; //a1*b2 - a2*b1

	float dot = octopcx * nvecx + octopcy * nvecy + octopcz * nvecz;

	if (dot < 0) {
		nvecx = -nvecx;
		nvecy = -nvecy;
		nvecz = -nvecz;
	}

	plane->nVecX = nvecx;
	plane->nVecY = nvecy;
	plane->nVecZ = nvecz;
}