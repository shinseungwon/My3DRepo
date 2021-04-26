#include "stdafx.h"
#include "TestTemplate.h"

TestTemplate::TestTemplate(HWND hWnd, WORD bWidth, WORD bHeight) : Space(hWnd, bWidth, bHeight){
	//set object
	rotationMatrix = new float* [4];
	for (int i = 0; i < 4; i++) {
		rotationMatrix[i] = new float[4];
	}
	setUpRotationMatrix(rotationMatrix, 3, 1, 1, 0); //angle x y z

	Object* obj1 = new Object();

	obj1->points->push_back(new pt(100, 150, 150));
	obj1->points->push_back(new pt(0, 0, 50));
	obj1->points->push_back(new pt(150, 0, 50));
	obj1->points->push_back(new pt(100, 125, -100));

	int ps = obj1->points->size();
	for (int i = 1; i < ps; i++) {
		for (int j = i + 1; j <= ps; j++) {
			obj1->edges->push_back(new ed(i, j));
		}
	}

	//12 13 14 23 24 34
	pl* tpl;

	//1 2 3 - 12 13 23
	tpl = new pl();
	tpl->pts->push_back(1);
	tpl->pts->push_back(2);
	tpl->pts->push_back(3);
	tpl->eds->push_back(1);
	tpl->eds->push_back(2);
	tpl->eds->push_back(4);
	tpl->color = 0x00ff0000;
	obj1->planes->push_back(tpl);

	//1 2 4 - 12 14 24
	tpl = new pl();
	tpl->pts->push_back(1);
	tpl->pts->push_back(2);
	tpl->pts->push_back(4);
	tpl->eds->push_back(1);
	tpl->eds->push_back(3);
	tpl->eds->push_back(5);
	tpl->color = 0x0000ff00;
	obj1->planes->push_back(tpl);

	//1 3 4 - 13 14 34
	tpl = new pl();
	tpl->pts->push_back(1);
	tpl->pts->push_back(3);
	tpl->pts->push_back(4);
	tpl->eds->push_back(2);
	tpl->eds->push_back(3);
	tpl->eds->push_back(6);
	tpl->color = 0x000000ff;
	obj1->planes->push_back(tpl);

	//2 3 4 - 23 24 34
	tpl = new pl();
	tpl->pts->push_back(2);
	tpl->pts->push_back(3);
	tpl->pts->push_back(4);
	tpl->eds->push_back(4);
	tpl->eds->push_back(5);
	tpl->eds->push_back(6);
	tpl->color = 0x00ff00ff;
	obj1->planes->push_back(tpl);

	objs->push_back(obj1);
	//~set object
}

void TestTemplate::Next() {
	//objs->at(0)->Pivot(rotationMatrix);
	objs->at(0)->Rotate(rotationMatrix);
}