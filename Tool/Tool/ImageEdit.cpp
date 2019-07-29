#include "stdafx.h"
#include "ImageEdit.h"
#include "MyImage.h"
#include "RGBByte.h"
#define RGB2GRAY(r, g, b) (0.299*(r) + 0.587*(g) + 0.114*(b))


void DibGrayscale(MyImage& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	MyImage cpy = dib;
	dib.CreateGrayImage(w, h);

	BYTE** ptr1 = dib.GetPtr();
	RGBBYTE** ptr2 = cpy.GetRGBPtr();

	BYTE r, g, b;
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++)
		{
			r = ptr2[j][i].r;
			g = ptr2[j][i].g;
			b = ptr2[j][i].b;

			ptr1[j][i] = (BYTE)limit(RGB2GRAY(r, g, b));
		}
	}
}

void DibBinarization(MyImage& dib, int th)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
		{
			ptr[j][i] = (ptr[j][i] > th) ? 255 : 0;
		}
}

void DibMorphologyErosion(MyImage& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	MyImage cpy = dib;;

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			if (ptr2[j][i] != 0)
			{
				if (ptr2[j - 1][i - 1] == 0 ||
					ptr2[j - 1][i] == 0 ||
					ptr2[j - 1][i + 1] == 0 ||
					ptr2[j][i - 1] == 0 ||
					ptr2[j][i + 1] == 0 ||
					ptr2[j + 1][i - 1] == 0 ||
					ptr2[j + 1][i] == 0 ||
					ptr2[j + 1][i + 1] == 0)
				{
					ptr1[j][i] = 0;
				}
			}
		}
}

void DibMorphologyDilation(MyImage& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	MyImage cpy = dib;;

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			if (ptr2[j][i] == 0)
			{
				if (ptr2[j - 1][i - 1] != 0 ||
					ptr2[j - 1][i] != 0 ||
					ptr2[j - 1][i + 1] != 0 ||
					ptr2[j][i - 1] != 0 ||
					ptr2[j][i + 1] != 0 ||
					ptr2[j + 1][i - 1] != 0 ||
					ptr2[j + 1][i] != 0 ||
					ptr2[j + 1][i + 1] != 0)
				{
					ptr1[j][i] = 255;
				}
			}
		}
}