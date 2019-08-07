#include "stdafx.h"
#include "ROI.h"
#include "MyImage.h"

ROI::ROI() : m_centerFlag(FALSE), m_roiFlag(FALSE), m_drawROI(FALSE)
{
}


ROI::~ROI()
{
}

void ROI::SetROI(CPoint pos, CPoint dPos)
{
	m_pos = pos;
	m_dPos = dPos;
}

void ROI::SetPos(CPoint m_pos)
{
	this->m_pos = m_pos;
}

void ROI::SetdPos(CPoint m_dPos)
{
	this->m_dPos = m_dPos;
}

CPoint ROI::GetPos()
{
	return m_pos;
}

CPoint ROI::GetdPos()
{
	return m_dPos;
}

void ROI::ROIDibBinarization(MyImage& image, int th)
{
	register int i, j;
	BYTE** ptr = image.GetPtr();


	for (j = m_pos.y*4 ; j < m_dPos.y*4; j++)
		for (i = m_pos.x*4; i < m_dPos.x*4; i++)
		{
			ptr[j][i] = (ptr[j][i] > th) ? 255 : 0;
		}
}

CPoint ROI::FindHole(MyImage & image, CPoint x1, CPoint x2)
{
	BYTE** ptr = image.GetPtr();

	int n_roiCenterX;
	int n_roiCenterY;
	CPoint top, bottom, left, right;
	// ROI 중점
	n_roiCenterX = (x2.x + x1.x) / 2;
	n_roiCenterY = (x2.y + x1.y) / 2;

	int x, y;

	x = n_roiCenterX;
	y = n_roiCenterY;
	while (true) {
		if (ptr[y][x] == 0) {
			left.x = x;
			left.y = y;
			break;
		}
		x = x - 1;
		if (x == x1.x) {
			break;
		}
	}

	x = n_roiCenterX;
	y = n_roiCenterY;
	while (true) {
		if (ptr[y][x] == 0) {
			right.x = x;
			right.y = y;
			break;
		}
		x = x + 1;
		if (x == x2.x) {
			break;
		}
	}

	x = n_roiCenterX;
	y = n_roiCenterY;
	while (true) {
		if (ptr[y][x] == 0) {
			top.x = x;
			top.y = y;
			break;
		}
		y = y - 1;
		if (y == x1.y) {
			break;
		}
	}

	x = n_roiCenterX;
	y = n_roiCenterY;
	while (true) {
		if (ptr[y][x] == 0) {
			bottom.x = x;
			bottom.y = y;
			break;
		}
		y = y + 1;
		if (y == x2.y) {
			break;
		}
	}

	//CPoint pos1(left.x, top.y);
	//CPoint pos2(right.x, top.y);
	//CPoint pos3(right.x, bottom.y);
	//CPoint pos4(left.x, bottom.y);
	x = (left.x + right.x) / 2;
	y = (top.y + bottom.y) / 2;

	//return GetIntersectPoint2D(top, bottom, left, right);
	return CPoint(x, y);
}

// ROI 설정 여부 확인
BOOL ROI::IsValid()
{
	return m_roiFlag;
}

CPoint ROI::CoordinateContingency(CPoint pos)
{
	CPoint temp;
	temp.x = pos.x * 4;
	temp.y = pos.y * 4;

	return temp;
}




void ROI::SetCenter(BOOL b)
{
	m_centerFlag = b;
}

BOOL ROI::IsCenter()
{
	return m_centerFlag;
}

void ROI::SetROI(BOOL b)
{
	m_roiFlag = b;
}




