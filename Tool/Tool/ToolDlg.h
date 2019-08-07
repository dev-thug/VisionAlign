
// ToolDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "MyImage.h"
#include "ROI.h"

// CToolDlg 대화 상자
class CToolDlg : public CDialogEx
{
// 생성입니다.
public:
	CToolDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 폰트 변수
	CFont m_font;
	
	// ROI 마우스 이벤트 처리 함수
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// 영상파일 로드 
	void LoadFile(MyImage& image);


	afx_msg void OnBnClickedButtonLoad0();
	afx_msg void OnBnClickedButtonLoad1();
	afx_msg void OnBnClickedButtonLoad2();
	afx_msg void OnBnClickedButtonLoad3();
private:
	// 이미지 객체
	MyImage m_oImage0, m_oImage1, m_oImage2, m_oImage3;
	MyImage m_image0, m_image1, m_image2, m_image3;
	// ROI 객체
	ROI m_roi0, m_roi1, m_roi2, m_roi3;
	CRect rect;
	CPoint m_pos, m_sPos, m_ePos;
	int m_ROIFlags;
	bool m_ROIDrag;
	bool m_dragFlag;
	int bin;
	int m_nTh0, m_nTh1, m_nTh2, m_nTh3;
	CPoint m_val0, m_val1, m_val2, m_val3;
	CPoint hole0, hole1, hole2, hole3;
public:

	//영상 이진화
	void Binarization(MyImage& image, int i);
	void ROIBinarization(MyImage& image, ROI& roi, int i);
	afx_msg void OnBnClickedButtonBin0();
	afx_msg void OnBnClickedButtonRoi0();

	// ROI 영역 그리기
	void SetROIPos(int nID, CPoint point);
	void SetROIdPos(int nID, CPoint point);

	afx_msg void OnBnClickedButtonRoi1();
	afx_msg void OnBnClickedButtonRoi2();
	afx_msg void OnBnClickedButtonRoi3();

	afx_msg void OnBnClickedButtonBin1();
	afx_msg void OnBnClickedButtonBin2();
	afx_msg void OnBnClickedButtonBin3();

	afx_msg void OnBnClickedButtonHole0();
	afx_msg void OnBnClickedButtonHole1();
	afx_msg void OnBnClickedButtonHole2();
	afx_msg void OnBnClickedButtonHole3();

	FLOAT Eval(CPoint pos1, CPoint pos2);
	afx_msg void OnBnClickedButton3();

	FLOAT EvalX(CPoint pos1, CPoint pos2);
	FLOAT EvalY(CPoint pos1, CPoint pos2);

	void SettingLoad();
	void SettingSave();
};
