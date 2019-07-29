#include "stdafx.h"
#include "ROI.h"


ROI::ROI()
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

void ROI::ROIDibBinarization(MyImage & dib, int th)
{
}

CPoint ROI::FindHole(MyImage & dib)
{
	return CPoint();
}


// ROI 설정 여부 확인
BOOL ROI::IsValid()
{
	return m_pos.x != m_dPos.x && m_dPos.y != m_dPos.y;
}
