#pragma once

#include<opencv\cv.h>


using namespace cv;
using namespace std;

class SkinDetector {
public:
	SkinDetector(void);

	void drawSkinColorSampler(Mat input);
	void calibrateInPosition(Mat input,int position);
	Mat getSkinMask(Mat input);

private:
	int hLowThreshold = 0;
	int hHighThreshold = 0;
	int sLowThreshold = 0;
	int sHighThreshold = 0;
	int vLowThreshold = 0;
	int vHighThreshold = 0;

	bool calibrate = false;
	bool calibratedLeft = false;
	bool calibratedMiddle = false;
	bool calibratedRight = false;

	Rect skinColorSamplerRectangle1, skinColorSamplerRectangle2;
	Rect skinColorSamplerRectangles[6];
	Scalar skinRectanglesAverageValues[6];


	void calibrating();
	void calculateThresholds(Mat sample1, Mat sample2);
	void SkinDetector::performOpening(Mat binaryImage, int structuralElementShapde, Point structuralElementSize);
};