#include "PensilDetector.h"
#include "opencv2\opencv.hpp"

#pragma warning(disable:4996)


PensilDetector::PensilDetector(void)
{
}

cv::Point PensilDetector::GetBottomPoint(vector<cv::Point> points)
{
	cv::Point tempPoint = cv::Point(0, 0);
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].y > tempPoint.y)
		{
			tempPoint = points[i];
		}
	}
	return tempPoint;
}

void PensilDetector::DrawRotateRect(cv::Mat frame, cv::RotatedRect rRect)
{
	cv::Point2f vertices[4];
	rRect.points(vertices);
	for (int i = 0; i < 4; i++)
		line(frame, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);
}

cv::Point PensilDetector::DrawPensil(cv::Mat frame)
{
	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	findContours(frame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	if (contours.size() == 0)
		return NULL;
	cv::Mat drawing = cv::Mat::zeros(frame.size(), CV_8UC3);
	char text[200] = "";
	CvFont font = cvFont(2, 2);

	int big = FindBiggestContour(contours);
	double epsilon = 0.1*arcLength(contours[big], true);
	vector<cv::Point> newVector;
	cv::approxPolyDP(cv::Mat(contours[big]),newVector, epsilon, true);

	//Rect brect = boundingRect(contours[big]);
	cv::RotatedRect brect = minAreaRect(contours[big]);
	
	//vector<Point2f> rectVector;
	cv::Point2f rectArray[4];
	vector<cv::Point> rectVector(2);
	brect.points(rectArray);
	//for (int i = 0; i < rectArray.size(); i++)
	{

	}
	rectVector[0].x = rectArray[0].x;
	rectVector[0].y = rectArray[0].y;
	rectVector[1].x = rectArray[2].x;
	rectVector[1].y = rectArray[2].y;
	DrawRotateRect(drawing, brect);
	int temp = cv::matchShapes(newVector, rectVector, 1, 0);
	cv::Moments mom1 = cv::moments(newVector, true);
	//cv::Moments mom2 = cv::moments(rectArray,true);
	int size = cv::contourArea(contours[big]);
	if ( temp>=0 && temp< 10 && size>100)
	{
		cv::Rect rect = cv::boundingRect(contours[big]);
		cv::Point bottomPoint = GetBottomPoint(contours[big]);
		circle(drawing, bottomPoint, 5, CV_RGB(255, 0, 0), 2, 8, 0);
		drawContours(drawing, contours, big, CV_RGB(255, 0, 0), 2, 8, hierarchy, 0, cv::Point());
		sprintf(text, "S = %d", temp);
		putText(drawing, text, cvPoint(rect.x + 20, rect.y + 20), 1, 1, CV_RGB(0, 255, 0));
		//rectangle(drawing, brect, CV_RGB(0, 0, 255), 3);
		//Rect brect = boundingRect(contours[big]);
		//rectangle(drawing, brect, CV_RGB(0, 0, 255), 3);
		imshow("Conturs", drawing);
		return bottomPoint;
	}
	return NULL;
}



int PensilDetector::FindBiggestContour(vector<vector<cv::Point> > contours)
{
	int indexOfBiggestContour = -1;
	int sizeOfBiggestContour = 0;

	for (int i = 0; i < contours.size(); i++) {
		if (contours[i].size() > sizeOfBiggestContour) {
			sizeOfBiggestContour = contours[i].size();
			indexOfBiggestContour = i;
		}
	}
	return indexOfBiggestContour;
}

