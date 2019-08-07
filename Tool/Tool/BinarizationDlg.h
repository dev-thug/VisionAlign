#pragma once
#include "afxcmn.h"
#include "MyImage.h"


// CBinarizationDlg 대화 상자입니다.

class CBinarizationDlg : public CDialog
{
	DECLARE_DYNAMIC(CBinarizationDlg)

public:
	CBinarizationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBinarizationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINARIZATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 슬라이더 제어 변수
	CSliderCtrl m_sliderThreshold;
	// 텍스트 제어 변수
	int m_nThreshold;
private:
	MyImage m_ImgSrc; // 입력 영상의 축소 복사본
	MyImage m_ImgRes; // 현재 임계값을 이용하여 m_DibSrc를 이진화한 영상
	CRect rect; // 이미지 윈도우 크기 변수


public:
	// 이미지변수 세터
	void SetImage(MyImage& image);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeThresholdEdit();
};
