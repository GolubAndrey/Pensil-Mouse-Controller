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
}

void SkinDetector::drawSkinColorSampler(cv::Mat input)
{
	int frameWidth = input.size().width, frameHeight = input.size().height;

	cv::Scalar rectangleColor = cv::Scalar(255, 0 , 255);

	skinColorSamplerRectangles[0] = cv::Rect(frameWidth / 13, frameHeight / 1.3, rectangleSize, rectangleSize);
	skinColorSamplerRectangles[1] = cv::Rect(frameWidth / 13, frameHeight / 4, rectangleSize, rectangleSize);
	skinColorSamplerRectangles[2] = cv::Rect(frameWidth / 2, frameHeight / 1.3, rectangleSize, rectangleSize);
	skinColorSamplerRectangles[3] = cv::Rect(frameWidth / 2, frameHeight / 4, rectangleSize, rectangleSize);
	skinColorSamplerRectangles[4] = cv::Rect(frameWidth / 1.05, frameHeight / 1.3, rectangleSize, rectangleSize);
	skinColorSamplerRectangles[5] = cv::Rect(frameWidth / 1.05, frameHeight / 4, rectangleSize, rectangleSize);

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

void SkinDetector::calibrateInPosition(cv::Mat input, int position)
{
	cv::Mat hsvInput;
	cv::cvtColor(input, hsvInput, CV_BGR2HSV);

	cv::Mat sample1 = cv::Mat(hsvInput, skinColorSamplerRectangles[position]);

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
int SkinDetector::minimal(int count,...)
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

int SkinDetector::maximal(int count, ...)
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

	int lowOffset = 20;
	int highOffset = 15;
	hLowThreshold = minimal(6, (int)skinRectanglesAverageValues[0][0], (int)skinRectanglesAverageValues[1][0],
		(int)skinRectanglesAverageValues[2][0], (int)skinRectanglesAverageValues[3][0],
		(int)skinRectanglesAverageValues[4][0], (int)skinRectanglesAverageValues[5][0])-lowOffset*2;
	if (hLowThreshold < 0)
		hLowThreshold = 0;
	hHighThreshold = maximal(6, (int)skinRectanglesAverageValues[0][0], (int)skinRectanglesAverageValues[1][0],
		(int)skinRectanglesAverageValues[2][0], (int)skinRectanglesAverageValues[3][0],
		(int)skinRectanglesAverageValues[4][0], (int)skinRectanglesAverageValues[5][0]);
	sLowThreshold = minimal(6, (int)skinRectanglesAverageValues[0][1], (int)skinRectanglesAverageValues[1][1],
		(int)skinRectanglesAverageValues[2][1], (int)skinRectanglesAverageValues[3][1],
		(int)skinRectanglesAverageValues[4][1], (int)skinRectanglesAverageValues[5][1])-2*lowOffset;
	/*sHighThreshold = maximal(6, (int)skinRectanglesAverageValues[0][1], (int)skinRectanglesAverageValues[1][1],
		(int)skinRectanglesAverageValues[2][1], (int)skinRectanglesAverageValues[3][1],
		(int)skinRectanglesAverageValues[4][1], (int)skinRectanglesAverageValues[5][1])+ highOffset;*/
	sHighThreshold = 255;
	vLowThreshold = minimal(6, (int)skinRectanglesAverageValues[0][2], (int)skinRectanglesAverageValues[1][2],
		(int)skinRectanglesAverageValues[2][2], (int)skinRectanglesAverageValues[3][2],
		(int)skinRectanglesAverageValues[4][2], (int)skinRectanglesAverageValues[5][2])-lowOffset;
	vHighThreshold = maximal(6, (int)skinRectanglesAverageValues[0][2], (int)skinRectanglesAverageValues[1][2],
		(int)skinRectanglesAverageValues[2][2], (int)skinRectanglesAverageValues[3][2],
		(int)skinRectanglesAverageValues[4][2], (int)skinRectanglesAverageValues[5][2])+ highOffset;
	vHighThreshold = 255;

	calibrate = true;
}

void SkinDetector::MorphOperations(cv::Mat &thresh)
{
	cv::Mat erodeElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::Mat dilateElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(8, 8));

	cv::erode(thresh, thresh, erodeElement);
	cv::dilate(thresh, thresh, dilateElement);
	cv::erode(thresh, thresh, erodeElement);
	cv::dilate(thresh, thresh, dilateElement);
}

cv::Mat SkinDetector::getSkinMask(cv::Mat input,int hLowThreshold, int sLowThreshold, int vLowThreshold, int hHighThreshold, int sHighThreshold, int vHighThreshold) {
	cv::Mat skinMask;

	cv::Mat hsvInput;
	cvtColor(input, hsvInput, CV_BGR2HSV);

	inRange(
		hsvInput,
		cv::Scalar(hLowThreshold, sLowThreshold, vLowThreshold),
		cv::Scalar(hHighThreshold, sHighThreshold, vHighThreshold),
		skinMask);

	//cv::GaussianBlur(skinMask, skinMask, cv::Size(3, 3), 0);
	//cv::dilate(skinMask, skinMask, 0);
	//cv::erode(skinMask, skinMask, 0);
	MorphOperations(skinMask);

	//performOpening(skinMask, MORPH_ELLIPSE, { 3, 3 });
	//dilate(skinMask, skinMask, Mat(), Point(-1, -1), 3);

	return skinMask;
}


void SkinDetector::performOpening(cv::Mat binaryImage, int kernelShape, cv::Point kernelSize) {
	cv::Mat structuringElement = cv::getStructuringElement(kernelShape, kernelSize);
	cv::morphologyEx(binaryImage, binaryImage, cv::MORPH_OPEN, structuringElement);
}