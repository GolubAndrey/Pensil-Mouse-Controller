#include "ColorDetector.h"
#include"opencv2\opencv.hpp"


ColorDetector::ColorDetector(void)
{
	CreateTrackbars();
	iLowPensilH = 75;
	iLowPensilS = 113;
	iLowPensilV = 23;
	iHighPensilH = 125;
	iLowWUH = 160;
	iLowWUS = 122;
	iLowWUV = 50;
	iLowWDH = 15;
	iHighWDH = 35;
	iLowWDS = 140;
	iLowWDV = 50;
}

void ColorDetector::CreateTrackbars()
{
	cv::namedWindow("Color Settings", 0);
	cv::resizeWindow("Color Settings", 400, 900);

	cvCreateTrackbar("Low pensil hue", "Color Settings", &iLowPensilH, 179); //Hue (0 - 179)
	cvCreateTrackbar("Hiw pensil hue", "Color Settings", &iHighPensilH, 179);

	cvCreateTrackbar("Low pensil saturation", "Color Settings", &iLowPensilS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("High pensil saturation", "Color Settings", &iHighPensilS, 255);

	cvCreateTrackbar("Low pensil value", "Color Settings", &iLowPensilV, 255); //Value (0 - 255)
	cvCreateTrackbar("High pensil value", "Color Settings", &iHighPensilV, 255);

	cvCreateTrackbar("Low wheel up hue", "Color Settings", &iLowWUH, 179); //Hue (0 - 179)
	cvCreateTrackbar("Hiw wheel up hue", "Color Settings", &iHighWUH, 179);

	cvCreateTrackbar("Low wheel up saturation", "Color Settings", &iLowWUS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("High wheel up saturation", "Color Settings", &iHighWUS, 255);

	cvCreateTrackbar("Low wheel up value", "Color Settings", &iLowWUV, 255); //Value (0 - 255)
	cvCreateTrackbar("High wheel up value", "Color Settings", &iHighWUV, 255);

	cvCreateTrackbar("Low wheel down hue", "Color Settings", &iLowWDH, 179); //Hue (0 - 179)
	cvCreateTrackbar("High wheel down hue", "Color Settings", &iHighWDH, 179);

	cvCreateTrackbar("Low wheel down saturation", "Color Settings", &iLowWDS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("High wheel down saturation", "Color Settings", &iHighWDS, 255);

	cvCreateTrackbar("Low wheel down value", "Color Settings", &iLowWDV, 255); //Value (0 - 255)
	cvCreateTrackbar("High wheel down value", "Color Settings", &iHighWDV, 255);
}

void ColorDetector::MorphOperations(cv::Mat &thresh)
{
	cv::Mat erodeElement = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
	cv::Mat dilateElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(8,8));

	cv::erode(thresh, thresh, erodeElement);
	cv::dilate(thresh, thresh, dilateElement);
	cv::dilate(thresh, thresh, dilateElement);
	cv::erode(thresh, thresh, erodeElement);
}

void ColorDetector::getPensilMask(cv::Mat &pensil,cv::Mat &wheelUp,cv::Mat &wheelDown)
{
	cvtColor(pensil, pensil, CV_BGR2HSV);

	inRange(
		pensil,
		cv::Scalar(iLowPensilH, iLowPensilS, iLowPensilV),
		cv::Scalar(iHighPensilH, iHighPensilS, iHighPensilV),
		pensil);

	MorphOperations(pensil);

	cvtColor(wheelDown, wheelDown, CV_BGR2HSV);

	inRange(
		wheelDown,
		cv::Scalar(iLowWDH, iLowWDS, iLowWDV),
		cv::Scalar(iHighWDH, iHighWDS, iHighWDV),
		wheelDown);

	cvtColor(wheelUp, wheelUp, CV_BGR2HSV);

	inRange(
		wheelUp,
		cv::Scalar(iLowWUH, iLowWUS, iLowWUV),
		cv::Scalar(iHighWUH, iHighWUS, iHighWUV),
		wheelUp);
}