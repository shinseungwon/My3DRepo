#include "stdafx.h"
#include "TestTemplate.h"

TestTemplate::TestTemplate(HWND hWnd, WORD bWidth, WORD bHeight) : Space(hWnd, bWidth, bHeight){
	//set object
	rotationMatrix = new float* [4];
	pivotMatrix = new float* [4];
	for (int i = 0; i < 4; i++) {
		rotationMatrix[i] = new float[4];
		pivotMatrix[i] = new float[4];
	}
	setUpRotationMatrix(rotationMatrix, 3, 1, 1, 0); //angle x y z
	setUpRotationMatrix(pivotMatrix, 2, 0, 0, 1); //angle x y z

	COLORREF* cs = new COLORREF[12];
	cs[0] = 0x000000ff;
	cs[1] = 0x0000ff00;
	cs[2] = 0x00ff0000;
	cs[3] = 0x00ffff00;
	cs[4] = 0x0000ffff;
	cs[5] = 0x00ff00ff;
	cs[6] = 0x00000088;
	cs[7] = 0x00008800;
	cs[8] = 0x00880000;
	cs[9] = 0x00888800;
	cs[10] = 0x00008888;
	cs[11] = 0x00880088;

	for (int i = 0; i < 3; i++) {
		Object* obj1 = new Object();

		obj1->points->push_back(new pt(300 - 250 * i, 150, 150));
		obj1->points->push_back(new pt(200 - 250 * i, 0, 50));
		obj1->points->push_back(new pt(350 - 250 * i, 0, 50));
		obj1->points->push_back(new pt(300 - 250 * i, 125, -100));

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
		tpl->color = cs[4 * i];
		obj1->planes->push_back(tpl);

		//1 2 4 - 12 14 24
		tpl = new pl();
		tpl->pts->push_back(1);
		tpl->pts->push_back(2);
		tpl->pts->push_back(4);
		tpl->eds->push_back(1);
		tpl->eds->push_back(3);
		tpl->eds->push_back(5);
		tpl->color = cs[4 * i + 1];
		obj1->planes->push_back(tpl);

		//1 3 4 - 13 14 34
		tpl = new pl();
		tpl->pts->push_back(1);
		tpl->pts->push_back(3);
		tpl->pts->push_back(4);
		tpl->eds->push_back(2);
		tpl->eds->push_back(3);
		tpl->eds->push_back(6);
		tpl->color = cs[4 * i + 2];
		obj1->planes->push_back(tpl);

		//2 3 4 - 23 24 34
		tpl = new pl();
		tpl->pts->push_back(2);
		tpl->pts->push_back(3);
		tpl->pts->push_back(4);
		tpl->eds->push_back(4);
		tpl->eds->push_back(5);
		tpl->eds->push_back(6);
		tpl->color = cs[4 * i + 3];
		obj1->planes->push_back(tpl);

		objs->push_back(obj1);
	}
	//~set object
}

void TestTemplate::Next() {	
	objs->at(0)->Pivot(pivotMatrix);
	objs->at(0)->Rotate(rotationMatrix);
	objs->at(1)->Rotate(rotationMatrix);
	objs->at(2)->Pivot(pivotMatrix);
	objs->at(2)->Rotate(rotationMatrix);
}