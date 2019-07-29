
#include "StdAfx.h"
#include "MyImage.h"
#include "RGBBYTE.h"
#define DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')

// 기본 생성자. 멤버 변수 초기화.
MyImage::MyImage()
{
	// 멤버 변수 초기화
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBitCount = 0;
	m_nDibSize = 0;

	m_pDib = NULL;

	m_pPtr = NULL;
	m_pRgbPtr = NULL;
}

// 복사 생성자. 인자로 넘어온 dib 객체를 복사한다.
MyImage::MyImage(const MyImage& dib)
{
	// 멤버 변수 초기화
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBitCount = 0;

	m_nDibSize = 0;

	m_pDib = NULL;

	m_pPtr = NULL;
	m_pRgbPtr = NULL;

	if (dib.m_pDib != NULL)
	{
		// 인자로부터 DIB 내용을 복사한다.
		m_pDib = new BYTE[dib.m_nDibSize];
		memcpy(m_pDib, dib.m_pDib, dib.m_nDibSize);

		// 멤버 변수 값 설정
		InitDib();
	}
}

// 소멸자. DIB 객체를 해제한다.
MyImage::~MyImage()
{
	Destroy();
}

// 지정한 크기의 그레이스케일 영상을 생성한다.  만약, 현재 DIB 객체를 가지고 있다면 삭제 후 생성한다.
BOOL MyImage::CreateGrayImage(int nWidth, int nHeight, BYTE value)
{
	// 현재 설정된 MyImage 객체가 있다면 삭제한다.
	if (m_pDib) Destroy();

	int nBitCount = 8;       // 그레이스케일 이미지의 비트 당 픽셀 수

							 // 실제 영상 데이터의 크기(바이트 단위)
	DWORD dwSizeImage = nHeight * (DWORD)((nWidth*nBitCount / 8 + 3)&~3);

	// (BITMAPINFOHEADER + 색상 테이블 + 픽셀 데이터)를 담을 메모리 공간 할당
	m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*(1 << nBitCount) + dwSizeImage];
	if (m_pDib == NULL) return FALSE;

	// BITMAPINFOHEADER 구조체 필드 값을 설정한다.
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;

	lpbmi->biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth = nWidth;
	lpbmi->biHeight = nHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = nBitCount;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;

	// 색상 테이블 값을 그레이스케일 형태로 설정한다.
	RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
	for (int i = 0; i < 256; i++)
	{
		pPal->rgbBlue = (BYTE)i;
		pPal->rgbGreen = (BYTE)i;
		pPal->rgbRed = (BYTE)i;
		pPal->rgbReserved = 0;
		pPal++;
	}

	// 멤버 변수 값 설정
	InitDib();

	// 픽셀 데이터 값 초기화
	BYTE* pData = (BYTE*)GetDibBitsAddr();
	memset(pData, value, dwSizeImage);

	return TRUE;
}

// 지정한 크기의 트루 컬러 영상을 생성한다. 만약, 현재 DIB 객체를 가지고 있다면 삭제 후 생성한다.
BOOL MyImage::CreateRGBImage(int nWidth, int nHeight, COLORREF value)
{
	// 현재 설정된 MyImage 객체가 있다면 삭제한다.
	if (m_pDib) Destroy();

	int nBitCount = 24;       // 트루칼라 이미지

							  // 실제 영상 데이터의 크기(바이트 단위)
	DWORD dwSizeImage = nHeight * (DWORD)((nWidth*nBitCount / 8 + 3)&~3);

	// (BITMAPINFOHEADER + 픽셀 데이터)를 담을 메모리 공간 할당
	m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) + dwSizeImage];
	if (m_pDib == NULL) return FALSE;

	// BITMAPINFOHEADER 구조체 필드 값을 설정한다.
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;

	lpbmi->biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth = nWidth;
	lpbmi->biHeight = nHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = nBitCount;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;

	// 멤버 변수 값 설정
	InitDib();

	// 픽셀 값 초기화
	register int i, j;
	for (j = 0; j < m_nHeight; j++)
		for (i = 0; i < m_nWidth; i++)
		{
			// RGBBYTE 클래스에서 = 는 operator overloading 되어있다.
			m_pRgbPtr[j][i] = value;
		}

	return TRUE;
}

// m_nWidth, m_nHeight, m_nBitCount등의 멤버 변수의 값을 설정한다. m_pDib가 BITMAPINFOHEADER의 시작번지를 가리키고 있는 상황에서 호출되어야 한다.
// m_pPtr, m_pRgbPtr 인터페이스 또한 생성한다.
BOOL MyImage::InitDib()
{
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;

	// 이미지의 가로 크기, 세로 크기, 픽셀당 비트수, DIB 크기에 대한
	// 멤버 변수를 설정한다.
	m_nWidth = lpbmi->biWidth;
	m_nHeight = lpbmi->biHeight;
	m_nBitCount = lpbmi->biBitCount;

	DWORD dwSizeImage = m_nHeight * (DWORD)((m_nWidth*m_nBitCount / 8 + 3)&~3);

	m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*GetPaletteNums()
		+ dwSizeImage;

	// m_pPtr 또는 m_pRgbPtr 멤버 변수를 설정한다.
	if (!AllocPtr())
		return FALSE;

	return TRUE;
}

// 동적으로 할당된 모든 데이터를 해제하고, NULL로 설정한다. 기타 멤버 변수도 초기화한다.
void MyImage::Destroy()
{
	if (m_pDib)
	{
		delete[] m_pDib;
		m_pDib = NULL;
	}

	FreePtr();  // m_pPtr 과 m_pRgbPtr 멤버 변수를 소멸시킨다.

				// 기타 멤버 변수 초기화
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBitCount = 0;
	m_nDibSize = 0;
}

// 인자로 넘어온 dib와 동일한 MyImage 객체를 생성한다.
MyImage& MyImage::operator=(const MyImage& dib)
{
	// 재귀 검사
	if (this == &dib)
		return *this;

	// dib가 비어있는 MyImage 객체라면 자신도 비운다.
	if (dib.m_pDib == NULL)
	{
		Destroy();
		return *this;
	}

	// 현재 설정된 MyImage 객체가 있다면 삭제한다.
	if (m_pDib) Destroy();

	// (BITMAPINFOHEADER + 픽셀 데이터)를 담을 메모리 공간 할당
	m_pDib = new BYTE[dib.m_nDibSize];

	// DIB 내용 복사
	memcpy(m_pDib, dib.m_pDib, dib.m_nDibSize);

	// 멤버 변수 값 설정
	InitDib();

	return *this;
}

// 인자로 넘어온 pDib가 가리키는 MyImage 객체를 복사한다.
BOOL MyImage::Copy(MyImage* pDib)
{
	// pDib가 비어있는 MyImage 객체를 가리키고 있다면 자신도 비운다.
	if (!pDib->IsValid())
	{
		Destroy();
		return TRUE;
	}

	// 현재 설정된 MyImage 객체가 있다면 삭제한다.
	if (m_pDib) Destroy();

	// DIB를 담을 메모리 공간 할당
	DWORD dwDibSize = pDib->GetDibSize();
	m_pDib = new BYTE[dwDibSize];
	if (m_pDib == NULL)
		return FALSE;

	// DIB 내용 복사
	BYTE* pbi = (BYTE*)pDib->GetBitmapInfoAddr();
	memcpy(m_pDib, pbi, dwDibSize);

	// 멤버 변수 값 설정
	InitDib();

	return TRUE;
}

// BMP 파일을 MyImage 객체로 불러온다. 확장자를 검사하여 BMP 파일이면 LoadBMP 함수를 다시 호출한다.
BOOL MyImage::Load(LPCTSTR lpszFileName)
{
	CString filetype;
	filetype = lpszFileName;
	filetype.MakeUpper();

	if (filetype.Right(4) == ".BMP")
		return LoadBMP(lpszFileName);
	else
		return FALSE;
}

// MyImage 객체를 BMP 파일로 저장한다. 확장자를 검사하여 BMP 파일이면 SaveBMP 함수를 다시 호출한다.
BOOL MyImage::Save(LPCTSTR lpszFileName)
{
	CString filetype;
	filetype = lpszFileName;
	filetype.MakeUpper();

	if (filetype.Right(4) == ".BMP")
		return SaveBMP(lpszFileName);
	else
		return FALSE;
}

//  출력 장치 좌표 (0,0) 위치에 DIB를 출력한다.
void MyImage::Draw(HDC hDC)
{
	Draw(hDC, 0, 0);
}


/*
인자 
hDC - 출력 장치의 DC 핸들.
dx  - 출력 장치 위치의 x 좌표.
dy  - 출력 장치 위치의 y 좌표.
*/

// 출력 장치 좌표 (dx,dy) 위치에 DIB를 출력한다.
void MyImage::Draw(HDC hDC, int dx, int dy)
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	void* lpBits = (void*)GetDibBitsAddr();

	::SetDIBitsToDevice(hDC,	// hDC
		dx,					// DestX
		dy,					// DestY
		m_nWidth,			// nSrcWidth
		m_nHeight,			// nSrcHeight
		0,					// SrcX
		0,					// SrcY
		0,					// nStartScan
		m_nHeight,			// nNumScans
		lpBits,				// lpBits
		lpbi,				// lpBitsInfo
		DIB_RGB_COLORS);	// wUsage
}

/*
인자
hDC - 출력 장치의 DC 핸들.
dx  - 출력 장치 위치의 x 좌표.
dy  - 출력 장치 위치의 y 좌표.
dw  - 출력할 공간의 가로 크기
dh  - 출력할 공간의 세로 크기
dwRop - 래스터 오퍼레이션 지정.
*/

// 출력 장치 좌표 (dx,dy) 위치에 (dw, dh) 크기로 DIB를 출력한다.
void MyImage::Draw(HDC hDC, int dx, int dy, int dw, int dh, DWORD dwRop)
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPSTR lpDIBBits = (LPSTR)GetDibBitsAddr();

	::StretchDIBits(hDC,	// hDC
		dx,					// XDest
		dy,					// YDest
		dw,					// nDestWidth
		dh,					// nDestHeight
		0,					// XSrc
		0,					// YSrc
		m_nWidth,			// nSrcWidth
		m_nHeight,			// nSrcHeight
		lpDIBBits,			// lpBits
		lpbi,				// lpBitsInfo
		DIB_RGB_COLORS,		// wUsage
		SRCCOPY);			// dwROP
}


/*
인자
hDC - 출력 장치의 DC 핸들.
dx  - 출력 장치 위치의 x 좌표.
dy  - 출력 장치 위치의 y 좌표.
dw  - 출력할 공간의 가로 크기
dh  - 출력할 공간의 세로 크기
dwRop - 래스터 오퍼레이션 지정.
sx  - 원본 영상의 좌상귀 x 좌표.
sy  - 원본 영상의 좌상귀 y 좌표.
sw  - 원본 영상에서 출력할 영역의 가로 크기
sh  - 원본 영상에서 출력할 영역의 세로 크기
*/

void MyImage::Draw(HDC hDC, int dx, int dy, int dw, int dh,
	int sx, int sy, int sw, int sh, DWORD dwRop)
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPSTR lpDIBBits = (LPSTR)GetDibBitsAddr();

	::StretchDIBits(hDC,	// hDC
		dx,					// XDest
		dy,					// YDest
		dw,					// nDestWidth
		dh,					// nDestHeight
		sx,					// XSrc
		sy,					// YSrc
		sw,					// nSrcWidth
		sh,					// nSrcHeight
		lpDIBBits,			// lpBits
		lpbi,				// lpBitsInfo
		DIB_RGB_COLORS,		// wUsage
		SRCCOPY);			// dwROP
}

// m_pDib를 참고하여, 픽셀 데이터의 시작 주소를 반환한다.
LPVOID MyImage::GetDibBitsAddr()
{
	LPBITMAPINFOHEADER lpbmi;
	LPVOID lpDibBits;

	lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	lpDibBits = (LPVOID)((BYTE*)m_pDib + lpbmi->biSize
		+ sizeof(RGBQUAD)*GetPaletteNums());

	return lpDibBits;
}

// 팔레트에서 필요한 RGBQUAD 구조체 배열의 개수를 반환한다. 트루컬러이면 0을 반환한다.
int MyImage::GetPaletteNums()
{
	switch (m_nBitCount)
	{
	case 1:
		return 2;
	case 4:
		return 16;
	case 8:
		return 256;
	default:
		return 0;
	}
}

// m_pPtr, m_pRgbPtr 인터페이스를 생성한다. 픽셀 데이터가 bottom-up 형태로 저장되어있다고 가정한다.
BOOL MyImage::AllocPtr()
{
	if (m_nBitCount == 8) // 그레이스케일 이미지
	{
		if (m_pPtr) FreePtr();
		m_pPtr = new BYTE*[m_nHeight];
		if (!m_pPtr) return FALSE;

		int nRWidth = ((m_nWidth*m_nBitCount / 8 + 3)&~3);
		BYTE* pData = (BYTE*)GetDibBitsAddr();
		for (int i = 0; i < m_nHeight; i++)
			m_pPtr[i] = (BYTE*)(pData + (m_nHeight - i - 1)*nRWidth);
	}
	else if (m_nBitCount == 24)  // 트루칼라 이미지
	{
		if (m_pRgbPtr) FreePtr();
		m_pRgbPtr = new RGBBYTE*[m_nHeight];
		if (!m_pRgbPtr) return FALSE;

		int nRWidth = ((m_nWidth*m_nBitCount / 8 + 3)&~3);
		BYTE* pData = (BYTE*)GetDibBitsAddr();
		for (int i = 0; i < m_nHeight; i++)
			m_pRgbPtr[i] = (RGBBYTE*)(pData + (m_nHeight - i - 1)*nRWidth);
	}
	else
		return FALSE;

	return TRUE;
}

// m_pPtr, m_pRgbPtr 인터페이스를 제거한다.
void MyImage::FreePtr()
{
	if (m_pPtr != NULL)
	{
		delete[] m_pPtr;
		m_pPtr = NULL;
	}

	if (m_pRgbPtr != NULL)
	{
		delete[] m_pRgbPtr;
		m_pRgbPtr = NULL;
	}
}



/*
typedef struct tagBITMAPFILEHEADER { // bmfh
WORD    bfType;			// 비트맵을 표시 (항상 "BM")
DWORD   bfSize;			// 전체 파일 크기
WORD    bfReserved1;	// 예약된 값. 항상 0
WORD    bfReserved2;	// 예약된 값. 항상 0
DWORD   bfOffBits;		// BITMAPFILEHEADER에서 픽셀 데이터까지의 옵셋
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{ // bmih
DWORD  biSize;			// 이 구조체의 크기
LONG   biWidth;			// 이미지의 폭 (픽셀 단위)
LONG   biHeight;		// 이미지의 높이 (픽셀 단위)
WORD   biPlanes;		// 비트플레인 수 (항상 1)
WORD   biBitCount		// 픽셀 당 비트 수
DWORD  biCompression;	// 압축 유형
DWORD  biSizeImage;		// 픽셀 데이터의 크기(바이트 수)
LONG   biXPelsPerMeter;	// 가로 해상도
LONG   biYPelsPerMeter; // 세로 해상도
DWORD  biClrUsed;		// 실제 사용되는 색상 수
DWORD  biClrImportant;	// 중요한 색상 인덱스 (0이면 전체)
} BITMAPINFOHEADER;
*/

// BMP 파일을 읽어서 MyImage 객체를 생성한다.
BOOL MyImage::LoadBMP(LPCTSTR lpszFileName)
{
	CFile   file;
	UINT    nFileSize, nDibSize;

	BITMAPFILEHEADER    bmfh;
	LPBITMAPINFOHEADER  lpbmi;

	// 읽기 모드로 파일 열기
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::shareDenyWrite))
		return FALSE;

	// BMP 파일의 크기 (바이트 단위)
	nFileSize = (UINT)file.GetLength();

	// BITMAPFILEHEADER 읽기
	if (file.Read((LPSTR)&bmfh, sizeof(BITMAPFILEHEADER))
		!= sizeof(BITMAPFILEHEADER))
	{
		file.Close();
		return FALSE;
	}

	// BMP 파일임을 나타내는 "BM" 마커가 있는지 확인
	if (bmfh.bfType != DIB_HEADER_MARKER)
	{
		file.Close();
		return FALSE;
	}

	// Packed-DIB 메모리 블럭의 크기 계산
	nDibSize = nFileSize - sizeof(bmfh);

	// 현재 설정된 MyImage 객체가 있다면 삭제한다.
	if (m_pDib) Destroy();

	// 위에서 계산한 만큼의 메모리 할당
	m_pDib = new BYTE[nDibSize];
	if (m_pDib == NULL)
	{
		file.Close();
		return FALSE;
	}

	// 파일로부터 Packed-DIB 크기만큼을 읽기
	if (file.Read(m_pDib, nDibSize) != nDibSize)
	{
		delete[] m_pDib;
		m_pDib = NULL;
		file.Close();
		return FALSE;
	}

	// BITMAPINFOHEADER 버전을 확인한다. 
	lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	if (lpbmi->biSize != sizeof(BITMAPINFOHEADER))
	{
		delete[] m_pDib;
		m_pDib = NULL;
		file.Close();
		return FALSE;
	}

	// 이제 m_pDib는 DIB를 담고있는 버퍼의 시작 주소를 가리키고 있다. 
	// MyImage 클래스의 멤버 변수의 값을 설정한다.
	if (!InitDib())
	{
		delete[] m_pDib;
		m_pDib = NULL;
		file.Close();
		return FALSE;
	}

	// 파일 닫기
	file.Close();

	return TRUE;
}

// MyImage 객체를 BMP 파일 형식으로 저장한다.
BOOL MyImage::SaveBMP(LPCTSTR lpszFileName)
{
	CFile	file;
	BITMAPFILEHEADER    bmfh;

	// DIB 객체를 포함하고 있지 않으면 FALSE를 반환.
	if (m_pDib == NULL)
		return FALSE;

	// 쓰기 모드로 파일 열기
	if (!file.Open(lpszFileName, CFile::modeCreate | CFile::modeWrite |
		CFile::shareDenyWrite))
		return FALSE;

	// BITMAPFILEHEADER 멤버 변수 설정
	bmfh.bfType = DIB_HEADER_MARKER;	// 항상 "BM"
	bmfh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + m_nDibSize;
	bmfh.bfReserved1 = 0;		// 항상 0
	bmfh.bfReserved2 = 0;		// 항상 0
	bmfh.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*GetPaletteNums());

	// BITMAPFILEHEADER 쓰기 
	file.Write((LPSTR)&bmfh, sizeof(bmfh));

	// DIB 구조(BITMAPINFOHEADER + 색상 테이블 + 픽셀 데이터) 쓰기
	file.Write(m_pDib, m_nDibSize);

	// 파일 닫기
	file.Close();

	return TRUE;
}

