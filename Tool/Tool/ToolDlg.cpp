
// ToolDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "ToolDlg.h"
#include "afxdialogex.h"
#include "BinarizationDlg.h"
#include "ImageEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CToolDlg 대화 상자



CToolDlg::CToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD0, &CToolDlg::OnBnClickedButtonLoad0)
	ON_BN_CLICKED(IDC_BUTTON_LOAD1, &CToolDlg::OnBnClickedButtonLoad1)
	ON_BN_CLICKED(IDC_BUTTON_LOAD2, &CToolDlg::OnBnClickedButtonLoad2)
	ON_BN_CLICKED(IDC_BUTTON_LOAD3, &CToolDlg::OnBnClickedButtonLoad3)
	ON_BN_CLICKED(IDC_BUTTON_BIN0, &CToolDlg::OnBnClickedButtonBin0)
	ON_BN_CLICKED(IDC_BUTTON_ROI0, &CToolDlg::OnBnClickedButtonRoi0)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CToolDlg 메시지 처리기

BOOL CToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_font.CreatePointFont(180, (LPCTSTR)"굴림");

	GetDlgItem(IDC_STATIC)->SetFont(&m_font, TRUE);
	GetDlgItem(IDC_STATIC_1)->SetFont(&m_font, TRUE);
	GetDlgItem(IDC_STATIC_2)->SetFont(&m_font, TRUE);
	GetDlgItem(IDC_STATIC_3)->SetFont(&m_font, TRUE);

	// 픽쳐 박스의 가로 세로의 길이를 구함
	CWnd* pImageWnd = GetDlgItem(IDC_IMAGE_WND0);
	pImageWnd->GetClientRect(rect);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CToolDlg::OnPaint()
{


	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		CDialogEx::OnPaint();

		if (m_image0.IsValid()) {
			CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND0));
			m_image0.Draw(dc2.m_hDC, 0, 0, rect.Width(), rect.Height());
		}
		if (m_image1.IsValid()) {
			CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND1));
			m_image1.Draw(dc2.m_hDC, 0, 0, rect.Width(), rect.Height());
		}
		if (m_image2.IsValid()) {
			CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND2));
			m_image2.Draw(dc2.m_hDC, 0, 0, rect.Width(), rect.Height());
		}
		if (m_image3.IsValid()) {
			CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND3));
			m_image3.Draw(dc2.m_hDC, 0, 0, rect.Width(), rect.Height());
		}


		if (roi0.IsValid()) {
			CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND0));
			CPen pen;
			pen.CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));    // ROI를 빨간색 대쉬펜 나타냄 
			CPen* oldPen = dc2.SelectObject(&pen);
			dc2.MoveTo(roi0.GetPos());
			dc2.LineTo(roi0.GetdPos());
			dc2.SelectObject(oldPen);
		}
		//if (m_image0.IsValid()) {
		//	CDC* p = m_wnd0.GetWindowDC();
		//	p->MoveTo(100, 200);
		//	p->LineTo(200, 200);
		//	CPaintDC dc2(GetDlgItem(IDC_IMAGE_WND0));
		//	//CStatic *staticSize = (CStatic *)GetDlgItem(IDC_IMAGE_WND0);
		//	CRect rect;
		//	CWnd* pImageWnd = GetDlgItem(IDC_IMAGE_WND0);

		//	
		//	pImageWnd->GetClientRect(rect);
		//	//staticSize->GetClientRect(rect);
		//	m_image0.Draw(p->m_hDC, 0, 0, rect.Width(), rect.Height());
		//	m_wnd0.ReleaseDC(p);
		//}
	}



}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 파일로드 리팩토링
void CToolDlg::LoadFile(MyImage & image)
{
	static TCHAR BASED_CODE szFilter[] = _T("BMP 파일(*.BMP) ||");

	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())
	{
		image.Load(dlg.GetPathName());
		Invalidate(FALSE);

	}

}

void CToolDlg::OnBnClickedButtonLoad0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LoadFile(m_image0);
}


void CToolDlg::OnBnClickedButtonLoad1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LoadFile(m_image1);
}


void CToolDlg::OnBnClickedButtonLoad2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LoadFile(m_image2);
}


void CToolDlg::OnBnClickedButtonLoad3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] = _T("BMP 파일(*.BMP) ||");
	LoadFile(m_image3);
}

void CToolDlg::Binarization(MyImage & image)
{
	// 영상이 로드 된 상태라면 수행하고 아니라면 메시지를 띄운다.
	if (m_image0.IsValid() == TRUE) {
		CBinarizationDlg dlg;
		dlg.SetImage(m_image0);
		if (dlg.DoModal() == IDOK) {

			// image0 원본 영상을 이진화를 수행한다. 
			DibBinarization(image, dlg.m_nThreshold);
			// 이진화에 사용된 임계값 int형을 CString 형으로 형변환 후 출력한다.
			CStatic * pStaticTest = (CStatic *)GetDlgItem(IDC_STATIC0);
			CString str;
			str.Format(_T("%d"), dlg.m_nThreshold);
			pStaticTest->SetWindowText(str);


			Invalidate(FALSE);
		}
	}
	else {
		MessageBox(_T("이미지를 로드 해주세요."));
	}
}

// 이진화 버튼 동작
void CToolDlg::OnBnClickedButtonBin0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Binarization(m_image0);
}


void CToolDlg::OnBnClickedButtonRoi0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!roi0.IsValid()) {
		CPoint temp(1,1);
		roi0.SetdPos(temp);
	}

}


void CToolDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!roi0.IsValid()) {
		roi0.SetPos(point);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CToolDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (roi0.IsValid()) {
		roi0.SetdPos(point);
		Invalidate(FALSE);
	}
	
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CToolDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}
