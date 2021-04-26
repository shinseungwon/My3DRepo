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
}

void Space::Clear() {
	memset(board, 0, (DWORD64)bWidth * bHeight * sizeof(COLORREF));
}

void Space::Draw() {
	int i, j, k, l, t, hw = bWidth / 2, hh = bHeight / 2, yInt;
	float x, y, z, dx, dy, x1, x2, y1, y2, ty, minY, maxY, xs, xb, ys, yb;

	vector<float>* meetXs = new vector<float>();

	for (i = 0; i < objs->size(); i++) {

		//dot
		vector<pt*>* points = objs->at(i)->points;
		for (j = 0; j < points->size(); j++) {
			pt* tpt = points->at(j);
			x = tpt->x;
			y = tpt->y;
			z = tpt->z;
			if (y + hh < bHeight && x + hw < bWidth) {
				t = ((int)y + hh) * bWidth + ((int)x + hw);
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

			if (abs(dx) >= abs(dy)) {

				if (x1 > x2) {
					swap(x1, x2);
					swap(y1, y2);
				}

				//for (x = x1 + 1; x < x2; x++) {
				for (k = ceil(x1); k < x2; k++) {
					y = y1 + dy * (k - x1) / dx;
					if (y + hh < bHeight && k + hw < bWidth) {
						t = ((int)y + hh) * bWidth + (k + hw);
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

				//for (y = y1 + 1; y < y2; y++) {
				for (k = ceil(y1); k < y2; k++) {
					x = x1 + dx * (k - y1) / dy;
					if (k + hh < bHeight && x + hw < bWidth) {
						t = (k + hh) * bWidth + ((int)x + hw);
						if (t >= 0 && t < size) {
							board[t] = 0x00ffffff;
						}
					}
				}
			}
		}

		//plane
		vector<pl*>* planes = objs->at(i)->planes;
		for (j = 0; j < planes->size(); j++) {

			pl* tpl = planes->at(j);			
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
				for (yInt = (int)ceil(minY); yInt <= (int)floor(maxY); yInt++) {
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

						xs = x1 > x2 ? x2 : x1;
						xb = x1 > x2 ? x1 : x2;
						ys = y1 > y2 ? y2 : y1;
						yb = y1 > y2 ? y1 : y2;

						if (y2 != y1) {
							if (x2 == x1) {
								x = x1;
								if (yInt >= ys && yInt <= yb) {
									meetXs->push_back(x);
								}
							}
							else {
								x = ((yInt - y1) * (x2 - x1) / (y2 - y1)) + x1;
								if (x <= xb && x >= xs) {
									meetXs->push_back(x);
								}
							}							
						}
					}

					if (meetXs->size() % 2 == 0) {
						sort(meetXs->begin(), meetXs->end());
						for (k = 0; k < meetXs->size() - 1; k += 1) {
							int x1 = meetXs->at(k);
							int x2 = meetXs->at(k + 1);
							for (x = x1 + 1; x <= x2 - 1; x++) {
								if (yInt + hh < bHeight && x + hw < bWidth) {
									t = (yInt + hh) * bWidth + ((int)x + hw);
									if (t >= 0 && t < size && (board[t] == 0x00000000 || board[t] == 0x00ffffff)) {
										if (meetXs->size() == 3) {
											board[t] = 0x0000ffff;
										}
										else board[t] = tpl->color;
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

void Space::Next() {
	cout << "Space::Next" << endl;
};