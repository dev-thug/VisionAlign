// BinarizationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "BinarizationDlg.h"
#include "afxdialogex.h"
#include "ImageEdit.h";
#include "ROI.h"


// CBinarizationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBinarizationDlg, CDialog)

CBinarizationDlg::CBinarizationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BINARIZATION, pParent)
	, m_nThreshold(128)
{

}

CBinarizationDlg::~CBinarizationDlg()
{
}

void CBinarizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THRESHOLD_SLIDER, m_sliderThreshold);
	DDX_Text(pDX, IDC_THRESHOLD_EDIT, m_nThreshold);
}


BEGIN_MESSAGE_MAP(CBinarizationDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_THRESHOLD_EDIT, &CBinarizationDlg::OnEnChangeThresholdEdit)
END_MESSAGE_MAP()


// CBinarizationDlg 메시지 처리기입니다.


// 이미지변수 Set
void CBinarizationDlg::SetImage(MyImage& image)
{
	m_ImgSrc = image;
}


BOOL CBinarizationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//  슬라이더 컨트롤을 초기하 한다.
	m_sliderThreshold.SetRange(0, 255);
	m_sliderThreshold.SetTicFreq(32);
	m_sliderThreshold.SetPageSize(32);
	m_sliderThreshold.SetPos(m_nThreshold);
	
	// 픽쳐 박스의 가로 세로의 길이를 구한다.
	CWnd* pImageWnd = GetDlgItem(IDC_IMAGE_WND);
	pImageWnd->GetClientRect(rect);

	// 이미지의 복사본을 만든다.
	m_ImgRes = m_ImgSrc;

	// 복사본을 이진화 한다.
	DibBinarization(m_ImgRes, m_nThreshold);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CBinarizationDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.


   // 복사본은 출력한다.
	CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND));
	m_ImgRes.Draw(dc2.m_hDC, 0, 0, rect.Width(), rect.Height());
}


void CBinarizationDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 슬라이드바에서 발생한 WM_HSCROLL 메시지인 경우 처리 한다.
	if (m_sliderThreshold.GetSafeHwnd() == pScrollBar->GetSafeHwnd()) {
		int nPos = m_sliderThreshold.GetPos();
		m_nThreshold = nPos;
		UpdateData(FALSE);
	}

	// 현재 설정된 임계값을 이용하여 복사본 영상의 이진화를 수행한다.
	m_ImgRes = m_ImgSrc;
	DibBinarization(m_ImgRes, m_nThreshold);
	Invalidate(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CBinarizationDlg::OnEnChangeThresholdEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 에디트 컨트롤에서 글자가 바뀐 경우, 슬라이더 컨트롤의 위치를 조절한다.
	UpdateData(TRUE);
	m_sliderThreshold.SetPos(m_nThreshold);

	// 현재 설정된 임계값을 이용하여 복사본 영상의 이진화를 수행한다.
	m_ImgRes = m_ImgSrc;
	DibBinarization(m_ImgRes, m_nThreshold);
	Invalidate(FALSE);
}
