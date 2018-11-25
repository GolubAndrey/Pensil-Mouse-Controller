#pragma once

#include<opencv\cv.h>

using namespace std;

class SkinDetector 
{
public:
	SkinDetector(void);

	void drawSkinColorSampler(cv::Mat input);
	void calibrateInPosition(cv::Mat input,int position);
	cv::Mat getSkinMask(cv::Mat input);

	bool calibrate = false;

private:
	int hLowThreshold = 0;
	int hHighThreshold = 0;
	int sLowThreshold = 0;
	int sHighThreshold = 0;
	int vLowThreshold = 0;
	int vHighThreshold = 0;

	const int rectangleSize = 5;


	bool allCalibrate[6];

	cv::Rect skinColorSamplerRectangles[6];
	cv::Scalar skinRectanglesAverageValues[6];


	void calibrating();
	bool CheckAllCalibrates();
	void MorphOperations(cv::Mat &threshold);
	void SkinDetector::performOpening(cv::Mat binaryImage, int structuralElementShapde, cv::Point structuralElementSize);
	int minimal(int count, ...);
	int maximal(int count, ...);
};