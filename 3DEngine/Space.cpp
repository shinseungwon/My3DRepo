#include "stdafx.h"
#include "Space.h"

Space::Space(HWND hWnd, WORD bWidth, WORD bHeight) {
	this->hWnd = hWnd;
	this->bWidth = bWidth;
	this->bHeight = bHeight;
	this->size = this->bWidth * this->bHeight;

	board = new COLORREF[(DWORD64)bWidth * bHeight];
	memset(board, 0, (DWORD64)bWidth * bHeight * sizeof(COLORREF));

	dim = new COLORREF * [bHeight];
	for (int i = 0; i < bHeight; i++) {
		dim[i] = &board[i * bWidth];
	}

	objs = new vector<Object*>();

	//set object
	Object* obj1 = new Object();

	obj1->points->push_back(new pt(300, 100, 200));
	obj1->points->push_back(new pt(-200, 150, 400));
	obj1->points->push_back(new pt(100, -100, 100));
	obj1->points->push_back(new pt(100, 50, -100));

	obj1->points->push_back(new pt(-100, -100, 100));
	obj1->points->push_back(new pt(100, -150, -250));
	obj1->points->push_back(new pt(-300, 200, -100));
	obj1->points->push_back(new pt(-150, -50, -200));

	int ps = obj1->points->size();
	for (int i = 1; i < ps; i++) {
		for (int j = i + 1; j <= ps; j++) {
			obj1->edges->push_back(new ed(i, j));
		}
	}

	objs->push_back(obj1);
	//~set object

	setUpRotationMatrix(1, 0, 1, 0);
}

void Space::Clear() {
	memset(board, 0, (DWORD64)bWidth * bHeight * sizeof(COLORREF));
}

void Space::Next() {	
	int sz = objs->at(0)->points->size();
	vector<pt*>* pts = objs->at(0)->points;
	for (int i = 0; i < sz; i++) {
		rotate(pts->at(i));
	}
}

void Space::Draw() {
	int i, j, k, x, y, z, dx, dy, x1, x2, y1, y2, t, hw = bWidth / 2, hh = bHeight / 2;
	for (i = 0; i < objs->size(); i++) {
		//dot
		vector<pt*>* points = objs->at(i)->points;
		for (j = 0; j < points->size(); j++) {
			pt* tpt = points->at(j);
			x = tpt->x;
			y = tpt->y;
			z = tpt->z;
			t = (y + hh) * bWidth + (x + hw);
			if (t >= 0 && t < size) {
				board[t] = 0x00ffffff;
			}			
		}

		//line
		vector<ed*>* edges = objs->at(i)->edges;
		for (j = 0; j < edges->size(); j++) {
			ed* ted = edges->at(j);

			pt* start = points->at(ted->start - 1);
			pt* end = points->at(ted->end - 1);

			x1 = start->x;
			y1 = start->y;

			x2 = end->x;
			y2 = end->y;

			dx = x2 - x1;
			dy = y2 - y1;

			if (abs(dx) > abs(dy)) {

				if (x1 > x2) {
					swap(x1, x2);
					swap(y1, y2);
				}

				for (x = x1 + 1; x < x2; x++) {
					y = y1 + dy * (x - x1) / dx;
					t = (y + hh) * bWidth + (x + hw);
					if (t >= 0 && t < size) {
						board[t] = 0x00ffffff;
					}
				}
			}
			else {

				if (y1 > y2) {
					swap(x1, x2);
					swap(y1, y2);
				}

				for (y = y1 + 1; y < y2; y++) {
					x = x1 + dx * (y - y1) / dy;
					t = (y + hh) * bWidth + (x + hw);
					if (t >= 0 && t < size) {
						board[t] = 0x00ffffff;
					}
				}
			}
		}
	}
}

void SpaceStart(Space* space, int w, int h) {
	promise<string> p1;
	future<string> data1 = p1.get_future();
	thread* t1 = new thread(SpaceDraw, space, &p1);
}

void SpaceDraw(Space* space, promise<string>* p) {
	HDC hdc = GetDC(space->hWnd);
	HDC src = CreateCompatibleDC(hdc);

	while (1) {
		space->Clear();
		space->Next();
		space->Draw();
		COLORREF* board = space->board;

		if (board != nullptr) {
			SelectObject(src, CreateBitmap(space->bWidth, space->bHeight, 1, 8 * 4, board));
			BitBlt(hdc, 0, 0, space->bWidth, space->bHeight, src, 0, 0, SRCCOPY);
		}
		Sleep(space->waiting);
	}
	DeleteDC(src);
	ReleaseDC(space->hWnd, hdc);
	p->set_value("End");
}

void Space::setUpRotationMatrix(float angle, float u, float v, float w)
{	
	float L = (u * u + v * v + w * w);
	angle = angle * 3.14 / 180.0;
	float u2 = u * u;
	float v2 = v * v;
	float w2 = w * w;

	rotationMatrix[0][0] = (u2 + (v2 + w2) * cos(angle)) / L;
	rotationMatrix[0][1] = (u * v * (1 - cos(angle)) - w * sqrt(L) * sin(angle)) / L;
	rotationMatrix[0][2] = (u * w * (1 - cos(angle)) + v * sqrt(L) * sin(angle)) / L;
	rotationMatrix[0][3] = 0.0;

	rotationMatrix[1][0] = (u * v * (1 - cos(angle)) + w * sqrt(L) * sin(angle)) / L;
	rotationMatrix[1][1] = (v2 + (u2 + w2) * cos(angle)) / L;
	rotationMatrix[1][2] = (v * w * (1 - cos(angle)) - u * sqrt(L) * sin(angle)) / L;
	rotationMatrix[1][3] = 0.0;

	rotationMatrix[2][0] = (u * w * (1 - cos(angle)) - v * sqrt(L) * sin(angle)) / L;
	rotationMatrix[2][1] = (v * w * (1 - cos(angle)) + u * sqrt(L) * sin(angle)) / L;
	rotationMatrix[2][2] = (w2 + (u2 + v2) * cos(angle)) / L;
	rotationMatrix[2][3] = 0.0;

	rotationMatrix[3][0] = 0.0;
	rotationMatrix[3][1] = 0.0;
	rotationMatrix[3][2] = 0.0;
	rotationMatrix[3][3] = 1.0;
}

void Space::rotate(pt* point) {
	float inputMatrix[4][1] = { point->x, point->y, point->z, 1.0 };
	float outputMatrix[4][1] = { 0.0, 0.0, 0.0, 0.0 };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 1; j++) {
			outputMatrix[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				outputMatrix[i][j] += rotationMatrix[i][k] * inputMatrix[k][j];
			}
		}
	}

	point->x = ceil(outputMatrix[0][0]);
	point->y = ceil(outputMatrix[1][0]);
	point->z = ceil(outputMatrix[2][0]);
}