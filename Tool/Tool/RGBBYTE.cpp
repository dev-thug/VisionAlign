#include "stdafx.h"
#include "MyImage.h"
#include "RGBBYTE.h"


// 생성자와 소멸자

RGBBYTE::RGBBYTE(const BYTE& pixel) : r(pixel), g(pixel), b(pixel)
{
}

RGBBYTE::RGBBYTE(const BYTE& _r, const BYTE& _g, const BYTE& _b)
	: r(_r), g(_g), b(_b)
{
}

RGBBYTE::RGBBYTE(const RGBBYTE& pixel) : r(pixel.r), g(pixel.g), b(pixel.b)
{
}

RGBBYTE::~RGBBYTE()
{
}

// 치환 연산자 오버로딩

RGBBYTE& RGBBYTE::operator=(const RGBBYTE& pixel)
{
	if (this == &pixel)
		return *this;

	this->r = pixel.r;
	this->g = pixel.g;
	this->b = pixel.b;

	return *this;
}

RGBBYTE& RGBBYTE::operator=(const COLORREF& pixel)
{
	r = (BYTE)(pixel);                  // GetRValue(pixel);
	g = (BYTE)((WORD)(pixel) >> 8);     // GetGValue(pixel);
	b = (BYTE)((pixel) >> 16);            // GetBValue(pixel);

	return *this;
}

RGBBYTE& RGBBYTE::operator=(const BYTE& pixel)
{
	r = pixel;
	g = pixel;
	b = pixel;

	return *this;
}

// 관계 연산자 오버로딩

int RGBBYTE::operator==(const RGBBYTE& pixel)
{
	if (r == pixel.r && g == pixel.g && b == pixel.b) return 1;
	else return 0;
}

int RGBBYTE::operator!=(const RGBBYTE& pixel)
{
	if (r != pixel.r || g != pixel.g || b != pixel.b) return 1;
	else return 0;
}

