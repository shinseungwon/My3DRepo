#include "stdafx.h"
#include "Space.h"

Space::Space(HWND hWnd, WORD bWidth, WORD bHeight) {
	this->hWnd = hWnd;
	this->bWidth = bWidth;
	this->bHeight = bHeight;

	board = new COLORREF[(DWORD64)bWidth * bHeight];
	memset(board, 0, (DWORD64)bWidth * bHeight * sizeof(COLORREF));

	//frame = new BYTE * [bHeight];
	//for (int i = 0; i < bHeight; i++) {
	//	frame[i] = new BYTE[bWidth];
	//	memset(frame[i], 0x00, bWidth * sizeof(BYTE));
	//}

	objs = new vector<Object*>();

	//set object
	Object* obj1 = new Object();

	obj1->points->push_back(new pt(300, 400, 200));
	obj1->points->push_back(new pt(200, 600, 400));
	obj1->points->push_back(new pt(400, 300, 100));
	obj1->points->push_back(new pt(400, 100, 100));

	obj1->edges->push_back(new ed(1, 2));
	obj1->edges->push_back(new ed(1, 3));
	obj1->edges->push_back(new ed(1, 4));

	obj1->edges->push_back(new ed(2, 3));
	obj1->edges->push_back(new ed(2, 4));

	obj1->edges->push_back(new ed(3, 4));

	objs->push_back(obj1);
}

void Space::Clear() {
	memset(board, 0, (DWORD64)bWidth * bHeight * sizeof(COLORREF));
}

void Space::Next() {	
	objs->at(0)->points->at(1)->x++;
}

void Space::Draw() {
	int i, j, x, y, z, dx, dy, x1, x2, y1, y2;
	for (i = 0; i < objs->size(); i++) {
		vector<pt*>* points = objs->at(i)->points;
		for (j = 0; j < points->size(); j++) {
			pt* tpt = points->at(j);
			x = tpt->x;
			y = tpt->y;
			z = tpt->z;
			board[y * bWidth + x] = 0x00ffffff;
		}

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
					board[y * bWidth + x] = 0x00ffffff;
				}
			}
			else {

				if (y1 > y2) {
					swap(x1, x2);
					swap(y1, y2);
				}

				for (y = y1 + 1; y < y2; y++) {
					x = x1 + dx * (y - y1) / dy;
					board[y * bWidth + x] = 0x00ffffff;
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

void DrawLine(Space* space, int x1, int y1, int x2, int y2) {

}