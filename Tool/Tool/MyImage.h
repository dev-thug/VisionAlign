#pragma once

class RGBBYTE;

class MyImage
{
private:
	LONG m_nWidth; // 이미지의 가로 크기(픽셀 단위)
	LONG m_nHeight; // 이미지의 세로 크기(픽셀 단위)
	WORD m_nBitCount; // 픽셀당 비트 수
	DWORD m_nDibSize; // (BITMAPINFOHEADER + 색상 테이블 + 픽셀 데이터) 크기

	BYTE* m_pDib; // DIB 내용을 담고 있는 버퍼의 시작 주소

	BYTE** m_pPtr; // 그레이스케일 이미지에서 2D 픽셀 접근용 포인터
	RGBBYTE** m_pRgbPtr; // 트루칼라 이미지에서 2D 픽셀 접근용 포인터


public:
	// 생성자 및 소멸자
	MyImage(void);
	MyImage(const MyImage& dib);
	~MyImage(void);

	// 이미지 생성 및 소멸
	BOOL CreateGrayImage(int nWidth, int nHeight, BYTE value = 255);
	BOOL CreateRGBImage(int nWidth, int nHeight, COLORREF value = 0x00ffffff);
	void Destroy();

	// 치환 연산자 오버로딩과 이미지 복사 
	MyImage& operator=(const MyImage& dib);
	BOOL Copy(MyImage* pImage);

	// 이미지 화면 출력
	void Draw(HDC hDC);
	void Draw(HDC hDC, int dx, int dy);
	void Draw(HDC hDC, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY);
	void Draw(HDC hDC, int dx, int dy, int dw, int dh,
		int sx, int sy, int sw, int sh, DWORD dwRop = SRCCOPY);

	// 파일 입출력
	BOOL Load(LPCTSTR lpszFileName);
	BOOL Save(LPCTSTR lpszFileName);

	// 2D 픽셀 포인터를 반환 (그레이스케일 & 트루칼라)
	BYTE** GetPtr();
	RGBBYTE** GetRGBPtr();

	// 이미지 정보 반환 함수
	LONG GetHeight() { return m_nHeight; }
	LONG GetWidth() { return m_nWidth; }
	WORD GetBitCount() { return m_nBitCount; }
	DWORD GetDibSize() { return m_nDibSize; }
	BOOL IsValid() { return (m_pDib != NULL); }

	// BITMAPINFOHEADER 시작 포인터 반환
	LPBITMAPINFO GetBitmapInfoAddr() { return (LPBITMAPINFO)m_pDib; }

	// 팔레트에서 RGBQUAD 배열의 개수를 반환
	int GetPaletteNums();

	// 픽셀 데이터 시작 포인터 반환
	LPVOID      GetDibBitsAddr();

protected:
	// m_pDib 설정 후, 이미지 정보 설정 함수
	BOOL InitDib();

	// BMP 파일 불러오기/저장하기
	BOOL LoadBMP(LPCTSTR lpszFileName);
	BOOL SaveBMP(LPCTSTR lpszFileName);

	// 2D 픽셀 포인터 접근을 위한 인터페이스 생성 & 소멸
	BOOL AllocPtr();
	void FreePtr();
};

inline BYTE** MyImage::GetPtr()
{
	ASSERT(m_nBitCount == 8);
	ASSERT(m_pPtr != NULL);

	return m_pPtr;
}

inline RGBBYTE** MyImage::GetRGBPtr()
{
	ASSERT(m_nBitCount == 24);
	ASSERT(m_pRgbPtr != NULL);

	return m_pRgbPtr;
}

// 템플릿 전역 함수 정의


/*************************************************************************
*
* 이름 : limit(const T& value)
* 인자 : value - 정수 혹은 실수.
* 반환 : 0~255 사이의 값.
* 설명 : value 값이 0보다 작으면 0으로, 255보다 크면 255로 변경한다.
*
*************************************************************************/

template<typename T>
inline T limit(const T& value)
{
	return ((value > 255) ? 255 : ((value < 0) ? 0 : value));
}

/*************************************************************************
*
* 이름 : limit(const T& value, const T& lower, const T& upper)
* 인자 : value - 정수 혹은 실수.
* 반환 : lower ~ upper 사이의 값.
* 설명 : value 값이 lower보다 작으면 lower으로,
*        upper보다 크면 upper로 변경한다.
*
*************************************************************************/

template<typename T>
inline T limit(const T& value, const T& lower, const T& upper)
{
	return ((value > upper) ? upper : ((value < lower) ? lower : value));
}

/*************************************************************************
*
* 이름 : swap(T& lha, T& rha)
* 인자 : lha - Left hand argument, rha - Right hand argument.
* 반환 : 없음.
* 설명 : rha와 lha의 값을 서로 바꾼다.
*
*************************************************************************/

template<typename T>
inline void swap(T& lha, T& rha)
{
	T tmp = lha;
	lha = rha;
	rha = tmp;
}

