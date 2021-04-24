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

	colors = new COLORREF[4];
	colors[0] = 0x00ff0000;//R
	colors[1] = 0x0000ff00;//G
	colors[2] = 0x000000ff;//B
	colors[3] = 0x00ffff00;//Y

	objs = new vector<Object*>();

	//set object
	Object* obj1 = new Object();

	obj1->points->push_back(new pt(0, 300, 300));
	obj1->points->push_back(new pt(-200, 0, 100));
	obj1->points->push_back(new pt(100, 0, 100));
	obj1->points->push_back(new pt(0, 250, -200));
	obj1->Center();

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
	obj1->planes->push_back(tpl);	

	//1 2 4 - 12 14 24
	tpl = new pl();
	tpl->pts->push_back(1);
	tpl->pts->push_back(2);
	tpl->pts->push_back(4);
	tpl->eds->push_back(1);
	tpl->eds->push_back(3);
	tpl->eds->push_back(5);
	obj1->planes->push_back(tpl);	

	//1 3 4 - 13 14 34
	tpl = new pl();
	tpl->pts->push_back(1);
	tpl->pts->push_back(3);
	tpl->pts->push_back(4);
	tpl->eds->push_back(2);
	tpl->eds->push_back(3);
	tpl->eds->push_back(6);
	obj1->planes->push_back(tpl);	

	//2 3 4 - 23 24 34
	tpl = new pl();
	tpl->pts->push_back(2);
	tpl->pts->push_back(3);
	tpl->pts->push_back(4);
	tpl->eds->push_back(4);
	tpl->eds->push_back(5);
	tpl->eds->push_back(6);
	obj1->planes->push_back(tpl);	

	objs->push_back(obj1);
	//~set object

	//angle x y z
	setUpRotationMatrix(1, 1, 1, 0);
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
	int i, j, k, l, m, n, t, hw = bWidth / 2, hh = bHeight / 2;
	float x, y, z, dx, dy, x1, x2, y1, y2, ty, minY, maxY, xs, xb;

	vector<int>* meetXs = new vector<int>();

	for (i = 0; i < objs->size(); i++) {

		//dot
		vector<pt*>* points = objs->at(i)->points;
		for (j = 0; j < points->size(); j++) {
			pt* tpt = points->at(j);
			x = (int)tpt->x;
			y = (int)tpt->y;
			z = (int)tpt->z;
			if (y + hh < bHeight && x + hw < bWidth) {
				t = (y + hh) * bWidth + (x + hw);
				if (t >= 0 && t < size) {
					board[t] = 0x00ffffff;
				}
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
					y = (int)(y1 + dy * (x - x1) / dx);
					if (y + hh < bHeight && x + hw < bWidth) {
						t = (y + hh) * bWidth + (x + hw);
						if (t >= 0 && t < size) {
							board[t] = 0x00ffffff;
						}
					}
				}
			}
			else {

				if (y1 > y2) {
					swap(x1, x2);
					swap(y1, y2);
				}

				for (y = y1 + 1; y < y2; y++) {
					x = (int)(x1 + dx * (y - y1) / dy);
					if (y + hh < bHeight && x + hw < bWidth) {
						t = (y + hh) * bWidth + (x + hw);
						if (t >= 0 && t < size) {
							board[t] = 0x00ffffff;
						}
					}
				}
			}
		}

		//plane
		objs->at(i)->Center();
		vector<pl*>* planes = objs->at(i)->planes;
		for (j = 0; j < planes->size(); j++) {

			COLORREF color = colors[j];
			pl* tpl = planes->at(j);
			//if (j != 0) continue;
			objs->at(i)->NVec(tpl);			
			
			if (tpl->nVecZ < 0) continue;

			minY = 999999;
			maxY = -999999;

			for (k = 0; k < tpl->pts->size(); k++) {
				ty = points->at(tpl->pts->at(k) - 1)->y;
				if (ty > maxY) {
					maxY = ty;
				}
				if (ty < minY) {
					minY = ty;
				}
			}

			if (minY != 999999 && maxY != -999999) {
				for (y = minY; y <= maxY; y++) {
					meetXs->clear();
					vector<int>* pts = tpl->pts;
					vector<int>* eds = tpl->eds;
					for (k = 0; k < eds->size(); k++) {
						pt* ab = points->at(edges->at(eds->at(k) - 1)->start - 1);
						pt* cd = points->at(edges->at(eds->at(k) - 1)->end - 1);

						x1 = ab->x;
						y1 = ab->y;
						x2 = cd->x;
						y2 = cd->y;

						if (x1 > x2) {
							xs = x2;
							xb = x1;
						}
						else {
							xs = x1;
							xb = x2;
						}

						if (y2 != y1) {
							x = ((y - y1) * (x2 - x1) / (float)(y2 - y1)) + x1;
							bool exist = false;
							for (l = 0; l < meetXs->size(); l++) {
								if (abs(meetXs->at(l) - x) <= 1) {
									exist = true;
								}
							}

							if (x <= xb && x >= xs && !exist) {
								meetXs->push_back(x);
							}
						}
					}

					if (meetXs->size() == 2) {
						sort(meetXs->begin(), meetXs->end());
						for (k = 0; k < meetXs->size() - 1; k += 2) {
							int x1 = meetXs->at(k);
							int x2 = meetXs->at(k + 1);
							for (x = x1 + 1; x <= x2 - 1; x++) {
								if (y + hh < bHeight && x + hw < bWidth) {
									t = (y + hh) * bWidth + (x + hw);
									if (t >= 0 && t < size && (board[t] == 0x00000000 || board[t] == 0x00ffffff)) {
										if (meetXs->size() == 3) {
											board[t] = 0x0000ffff;
										}
										else board[t] = color;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	meetXs->clear();
	delete meetXs;
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