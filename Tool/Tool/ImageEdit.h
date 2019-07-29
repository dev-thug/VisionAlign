#pragma once
class MyImage;

void DibGrayscale(MyImage& dib);
void DibBinarization(MyImage& dib, int th);
void DibMorphologyErosion(MyImage& dib);
void DibMorphologyDilation(MyImage& dib);