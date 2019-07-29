#pragma once

class RGBBYTE
{

// 멤버 변수 선언


public:
	BYTE b;
	BYTE g;
	BYTE r;

// 멤버 함수 선언
public:
	RGBBYTE(const BYTE& pixel = 0);     // 기본 생성자
	RGBBYTE(const BYTE& _r, const BYTE& _g, const BYTE& _b);
	RGBBYTE(const RGBBYTE& pixel);      // 복사 생성자
	~RGBBYTE();							// 소멸자

public:
	// 치환 연산자 오버로딩
	RGBBYTE&    operator=(const RGBBYTE& pixel);
	RGBBYTE&    operator=(const COLORREF& pixel);
	RGBBYTE&    operator=(const BYTE& pixel);

	// 관계 연산자 오버로딩
	int         operator==(const RGBBYTE& pixel);
	int         operator!=(const RGBBYTE& pixel);
};

