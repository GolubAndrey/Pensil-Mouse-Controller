#include "PensilDetector.h"
#include "opencv2\opencv.hpp"

#pragma warning(disable:4996)

vector<cv::Point> GetMiddleRectangle(vector<vector<cv::Point>> rectangles);

PensilDetector::PensilDetector(void)
{
	tempVector.resize(4);
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
	vector<int> convex;
	cv::convexHull(cv::Mat(contours[big]), convex, false);
	vector<cv::Vec4i> offsets;
	cv::convexityDefects(cv::Mat(contours[big]), convex, offsets);
	if (offsets.size() != 0)
	{
		bool flag = false;
		for (int i = 0; i < offsets.size(); i++)
		{
			if (offsets[i].val[3]/256>10)
			{
				flag = true;
				break;
			}
		}
		if (!flag && cv::contourArea(contours[big])>500)
		{
			cv::polylines(drawing, contours[big], true, cv::Scalar(0, 0, 255), 2);
			cv::Point bottomPoint = GetBottomPoint(contours[big]);
			circle(drawing, bottomPoint, 5, CV_RGB(255, 0, 0), 2, 8, 0);
			imshow("Conturs", drawing);
			return bottomPoint;
		}
	}
	//cv::polylines(drawing, convex, true, cv::Scalar(0, 0, 255), 2);
	//double epsilon = 0.1*arcLength(contours[big],true);
	//vector<cv::Point> newVector;
	//cv::approxPolyDP(cv::Mat(contours[big]),newVector, epsilon, true);
	//cv::polylines(drawing, newVector, true, cv::Scalar(0, 0, 255), 2);

	//cv::Rect brect = boundingRect(contours[big]);
	cv::RotatedRect brect = minAreaRect(contours[big]);
	//cv::RotatedRect brect = minAreaRect(newVector);
	
	cv::Point2f rectArray[4];
	brect.points(rectArray);

	
	//vector<cv::Point2f> rectArray(4);
//	int result = cv::matchShapes(contours[big], convex, 1, 0);
	//printf("%d\n", result);
	//DrawRotateRect(drawing, brect);
	//int temp = cv::matchShapes(newVector, rectVector, 1, 0);
	//cv::Moments mom1 = cv::moments(newVector, true);
	//int size = cv::contourArea(contours[big]);
	/*int size = cv::contourArea(newVector);
	{
		cv::Rect rect = cv::boundingRect(newVector);
		drawContours(drawing, contours, big, CV_RGB(255, 0, 0), 2, 8, hierarchy, 0, cv::Point());
		//sprintf(text, "S = %d", temp);
		//putText(drawing, text, cvPoint(rect.x + 20, rect.y + 20), 1, 1, CV_RGB(0, 255, 0));
		//rectangle(drawing, brect, CV_RGB(0, 0, 255), 3);
		//Rect brect = boundingRect(contours[big]);
		//rectangle(drawing, brect, CV_RGB(0, 0, 255), 3);
		imshow("Conturs", drawing);
		return bottomPoint;
	}*/
	imshow("Conturs", drawing);
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

int minimal(int count, ...)
{
	va_list valist;
	int temp;
	int min = 260;
	va_start(valist, count);
	for (; count; count--)
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
	for (; count; count--)
	{
		temp = va_arg(arg, int);
		if (temp>max)
		{
			max = temp;
		}
	}
	va_end(arg);
	return max;
}

vector<cv::Point> GetMiddleRectangle(vector<vector<cv::Point>> rectangles)
{
	vector<cv::Point> result(4);
	int sumX = 0;
	int sumY = 0;
	int i;
	int j;
	for (i = 0; i < rectangles.size(); i++)
	{
		sumX = 0;
		sumY = 0;
		for (j = 0; j < 4; j++)
		{
			sumX += rectangles[i][j].x;
			sumY += rectangles[i][j].y;
		}
		result[i] = cv::Point(sumX / j, sumY / j);
	}
	return result;
}



