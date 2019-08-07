#pragma once

class MyImage;

class ROI
{
private:
	CPoint m_pos, m_dPos; // ROI 영역 처음 좌표, 마지막 좌표
	BOOL m_centerFlag;
	BOOL m_roiFlag;
	
public:
	ROI();
	~ROI();

	// ROI 좌표 설정
	void SetROI(CPoint pos, CPoint dPos);
	void SetPos(CPoint m_pos);
	void SetdPos(CPoint m_dPos);
	CPoint GetPos();
	CPoint GetdPos();

	// ROI 영역 이진화
	void ROIDibBinarization(MyImage& image, int th);

	// 설정된 ROI 영역내에서 Hole의 중심점 좌표를 반환
	CPoint FindHole(MyImage& image, CPoint x1, CPoint x2);
	// ROI 설정 여부 확인
	BOOL IsValid();
	// 좌표 보정을 위한 함수
	CPoint CoordinateContingency(CPoint pos);

	void SetCenter(BOOL b);
	BOOL IsCenter();
	void SetROI(BOOL b);

	BOOL m_drawROI;
};

