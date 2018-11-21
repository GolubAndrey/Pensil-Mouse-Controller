#include "SkinDetector.h"
#include"opencv2\opencv.hpp"


SkinDetector::SkinDetector(void) 
{
	hLowThreshold = 0;
	hHighThreshold = 0;
	sLowThreshold = 0;
	sHighThreshold = 0;
	vLowThreshold = 0;
	vHighThreshold = 0;

	for (int i = 0; i < 6; i++)
		allCalibrate[i] = false;
	calibrate = false;
	skinColorSamplerRectangle1, skinColorSamplerRectangle2;
}

void SkinDetector::drawSkinColorSampler(Mat input) 
{
	int frameWidth = input.size().width, frameHeight = input.size().height;

	int rectangleSize = 5;
	Scalar rectangleColor = Scalar(255, 0 , 255);
	//5
	skinColorSamplerRectangles[0] = Rect(frameWidth / 13, frameHeight / 1.3, rectangleSize, rectangleSize);
	skinColorSamplerRectangles[1] = Rect(frameWidth / 13, frameHeight / 4, rectangleSize, rectangleSize);
	skinColorSamplerRectangles[2] = Rect(frameWidth / 2, frameHeight / 1.3, rectangleSize, rectangleSize);
	skinColorSamplerRectangles[3] = Rect(frameWidth / 2, frameHeight / 4, rectangleSize, rectangleSize);
	skinColorSamplerRectangles[4] = Rect(frameWidth / 1.05, frameHeight / 1.3, rectangleSize, rectangleSize);
	skinColorSamplerRectangles[5] = Rect(frameWidth / 1.05, frameHeight / 4, rectangleSize, rectangleSize);

	for (int i = 0; i < 6; i++)
	{
		rectangle(
			input,
			skinColorSamplerRectangles[i],
			rectangleColor
		);

		rectangle(
			input,
			skinColorSamplerRectangles[i],
			rectangleColor
		);
	}
}

void SkinDetector::calibrateInPosition(Mat input, int position)
{
	Mat hsvInput;
	cvtColor(input, hsvInput, CV_BGR2HSV);

	Mat sample1 = Mat(hsvInput, skinColorSamplerRectangles[position]);

	skinRectanglesAverageValues[position] = mean(sample1);

	if (position >= 0 && position < 6)
	{
		allCalibrate[position] = true;
	}

	if (calibrate = CheckAllCalibrates())
	{
		calibrating();
	}
}

bool SkinDetector::CheckAllCalibrates()
{
	for (int i = 0; i < 6; i++)
	{
		if (!allCalibrate[i])
			return false;
	}
	return true;
}

//template <typename...Args>
int minimal(int count,...)
{
	va_list valist;
	int temp;
	int min = 260;
	va_start(valist, count);
	for (;count;count--)
	{
		temp = va_arg(valist, int);
		if (temp<min)
		{
			min = temp;
		}
	}
	va_end(valist);
	return min;
}

int maximal(int count, ...)
{
	va_list arg;
	int temp;
	int max = 0;
	va_start(arg, count);
	for (;count;count--)
	{
		temp= va_arg(arg, int);
		if (temp>max)
		{
			max = temp;
		}
	}
	va_end(arg);
	return max;
}

void SkinDetector::calibrating() {

	int lowOffset = 15;
	int highOffset = 15;
	hLowThreshold = minimal(6, (int)skinRectanglesAverageValues[0][0], (int)skinRectanglesAverageValues[1][0],
		(int)skinRectanglesAverageValues[2][0], (int)skinRectanglesAverageValues[3][0],
		(int)skinRectanglesAverageValues[4][0], (int)skinRectanglesAverageValues[5][0])- lowOffset;
	hHighThreshold = maximal(6, (int)skinRectanglesAverageValues[0][0], (int)skinRectanglesAverageValues[1][0],
		(int)skinRectanglesAverageValues[2][0], (int)skinRectanglesAverageValues[3][0],
		(int)skinRectanglesAverageValues[4][0], (int)skinRectanglesAverageValues[5][0])+ highOffset;
	sLowThreshold = minimal(6, (int)skinRectanglesAverageValues[0][1], (int)skinRectanglesAverageValues[1][1],
		(int)skinRectanglesAverageValues[2][1], (int)skinRectanglesAverageValues[3][1],
		(int)skinRectanglesAverageValues[4][1], (int)skinRectanglesAverageValues[5][1])- lowOffset;
	sHighThreshold = maximal(6, (int)skinRectanglesAverageValues[0][1], (int)skinRectanglesAverageValues[1][1],
		(int)skinRectanglesAverageValues[2][1], (int)skinRectanglesAverageValues[3][1],
		(int)skinRectanglesAverageValues[4][1], (int)skinRectanglesAverageValues[5][1])+ highOffset;
	vLowThreshold = minimal(6, (int)skinRectanglesAverageValues[0][2], (int)skinRectanglesAverageValues[1][2],
		(int)skinRectanglesAverageValues[2][2], (int)skinRectanglesAverageValues[3][2],
		(int)skinRectanglesAverageValues[4][2], (int)skinRectanglesAverageValues[5][2])- lowOffset;
	vHighThreshold = maximal(6, (int)skinRectanglesAverageValues[0][2], (int)skinRectanglesAverageValues[1][2],
		(int)skinRectanglesAverageValues[2][2], (int)skinRectanglesAverageValues[3][2],
		(int)skinRectanglesAverageValues[4][2], (int)skinRectanglesAverageValues[5][2])+ highOffset;

	calibrate = true;
	//Mat hsvInput;
	//cvtColor(input, hsvInput, CV_BGR2HSV);

//	Mat sample1 = Mat(hsvInput, skinColorSamplerRectangle1);
//	Mat sample2 = Mat(hsvInput, skinColorSamplerRectangle2);

//	calculateThresholds(sample1, sample2);

	//calibrated = true;
}



void SkinDetector::calculateThresholds(Mat sample1, Mat sample2) {
	//int offsetLowThreshold = 70;
	//int offsetHighThreshold = 20;

	/*Scalar hsvMeansSample1 = mean(sample1);
	Scalar hsvMeansSample2 = mean(sample2);

	hLowThreshold = min(hsvMeansSample1[0], hsvMeansSample2[0]) - offsetLowThreshold;
	hHighThreshold = max(hsvMeansSample1[0], hsvMeansSample2[0]) + offsetHighThreshold;

	sLowThreshold = min(hsvMeansSample1[1], hsvMeansSample2[1]) - offsetLowThreshold;
	sHighThreshold = max(hsvMeansSample1[1], hsvMeansSample2[1]) + offsetHighThreshold;

	// the V channel shouldn't be used. By ignorint it, shadows on the hand wouldn't interfire with segmentation.
	// Unfortunately there's a bug somewhere and not using the V channel causes some problem. This shouldn't be too hard to fix.
	vLowThreshold = min(hsvMeansSample1[2], hsvMeansSample2[2]) - offsetLowThreshold;
	vHighThreshold = max(hsvMeansSample1[2], hsvMeansSample2[2]) + offsetHighThreshold;*/
	//vLowThreshold = 0;
	//vHighThreshold = 255;
}

Mat SkinDetector::getSkinMask(Mat input) {
	Mat skinMask;

	if (!calibrate) {
		skinMask = Mat::zeros(input.size(), CV_8UC1);
		return skinMask;
	}

	Mat hsvInput;
	cvtColor(input, hsvInput, CV_BGR2HSV);

	inRange(
		hsvInput,
		Scalar(hLowThreshold, sLowThreshold, vLowThreshold),
		Scalar(hHighThreshold, sHighThreshold, vHighThreshold),
		skinMask);

	//performOpening(skinMask, MORPH_ELLIPSE, { 3, 3 });
	//dilate(skinMask, skinMask, Mat(), Point(-1, -1), 3);

	return skinMask;
}

void SkinDetector::performOpening(Mat binaryImage, int kernelShape, Point kernelSize) {
	Mat structuringElement = getStructuringElement(kernelShape, kernelSize);
	morphologyEx(binaryImage, binaryImage, MORPH_OPEN, structuringElement);
}