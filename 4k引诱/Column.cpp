#include "Column.h"
using namespace Gdiplus;

Column::Column(int column, HWND window)
	: _column(column + 4) {
	GetClientRect(window, &_clientRectangle);
}

RectF Column::LineRectangle() {
	return RectF(_clientRectangle.right / 12 * _column - 942 / 10, _clientRectangle.bottom - 100, (942 - 20) / 10, (95 - 20) / 10);
}

RectF Column::TapRectangle(float sec) {
	return RectF(_clientRectangle.right / 12 * _column - 942 / 10, (sec + 4) * (_clientRectangle.bottom - 100) / 4, (942 - 20) / 10, (95 - 20) / 10);
}