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
	cv::Mat mat = cv::Mat::zeros(frame.size(), CV_8UC3);
	cv::Point2f vertices[4];
	rRect.points(vertices);
	for (int i = 0; i < 4; i++)
		line(mat, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);
	cv::imshow("as", mat);
}

void PensilDetector::DrawPensil(cv::Mat frame)
{
	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	findContours(frame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	if (contours.size() == 0)
		return;
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
			cv::Point tempPoint = cv::Point((contours[big][offsets[i].val[0]].x + contours[big][offsets[i].val[1]].x) / 2, (contours[big][offsets[i].val[0]].y + contours[big][offsets[i].val[1]].y) / 2);
			int length = sqrt(pow(abs(tempPoint.x - contours[big][offsets[i].val[2]].x), 2) + pow(abs(tempPoint.y - contours[big][offsets[i].val[2]].y), 2));
			if (length>50)
			{
				printf("%d\n", length);
				flag = true;
				break;
			}
		}
		if (!flag && cv::contourArea(contours[big])>500)
		{
			cv::polylines(drawing, contours[big], true, cv::Scalar(0, 0, 255), 2);
			mousePosition = GetBottomPoint(contours[big]);
			circle(drawing, mousePosition, 5, CV_RGB(255, 0, 0), 2, 8, 0);
			imshow("Conturs", drawing);
		}
	}
	//cv::polylines(drawing, convex, true, cv::Scalar(0, 0, 255), 2);
	//double epsilon = 0.1*arcLength(contours[big],true);
	//vector<cv::Point> newVector;
	//cv::approxPolyDP(cv::Mat(contours[big]),newVector, epsilon, true);
	//cv::polylines(drawing, newVector, true, cv::Scalar(0, 0, 255), 2);

	//cv::Rect brect = boundingRect(contours[big]);
	cv::RotatedRect brect = minAreaRect(contours[big]);
	CheckLeftAndRightClick(brect.angle);
	imshow("Conturs", drawing);
}

void PensilDetector::CalculatePositionAndClicks(cv::Mat frame)
{
	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	findContours(frame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	if (contours.size() == 0)
		return;

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
			cv::Point tempPoint = cv::Point((contours[big][offsets[i].val[0]].x + contours[big][offsets[i].val[1]].x) / 2, (contours[big][offsets[i].val[0]].y + contours[big][offsets[i].val[1]].y) / 2);
			
			if (sqrt(pow(abs(tempPoint.x - contours[big][offsets[i].val[2]].x), 2) + pow(abs(tempPoint.y - contours[big][offsets[i].val[2]].y), 2))>50)
			{
				flag = true;
				break;
			}
		}
		double area = cv::contourArea(contours[big]);
		//printf("%f\n", area);
		if (!flag && area>500)
		{
			mousePosition = GetBottomPoint(contours[big]);
			 
			cv::RotatedRect brect = cv::fitEllipse(contours[big]);
			double ang = brect.angle - 90;
			brect = minAreaRect(contours[big]);
			
			DrawRotateRect(frame, brect);
			
			//if (brect.size.width > brect.size.height)
			//	ang += 90;
			CheckLeftAndRightClick(ang);
			//
			//printf("%f\n", brect.angle);
		}
	}
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

void PensilDetector::CheckLeftAndRightClick(int angel)
{
	//printf("%d\n", angel);
	if (angel > 1 && angel<50)
	{
		if (leftClickFlag)
		{
			leftClickFlag = false;
		}
		if (timerRight)
		{
			rightClickFlag = false;
		}
		else
		{
			rightClickFlag = true;
		}
		if (timer == 0)
		{
			timerRight = true;
		}
		if (timerRight)
		{
			timer++;
		}
		if (timerRight > timerClick)
		{
			timer = 0;
			timerRight = false;
		}
	}
	else
	{
		if (angel > -50 && angel<-1)
		{
			if (rightClickFlag)
			{
				rightClickFlag = false;
			}
			if (timerLeft)
			{
				leftClickFlag = false;
			}
			else
			{
				leftClickFlag = true;
			}
			if (timer == 0)
			{
				timerLeft = true;
			}
			if (timerLeft)
			{
				timer++;
			}
			if (timerLeft > timerClick)
			{
				timer = 0;
				timerLeft = false;
			}
		}
		else
		{
				rightClickFlag = false;
				timerRight = false;
				timer = 0;
				leftClickFlag = false;
				timerLeft = false;
				timer = 0;
		}
	}
}

bool PensilDetector::GetLeftClick()
{
	return leftClickFlag;
}

bool PensilDetector::GetRightClick()
{
	return rightClickFlag;
}

cv::Point PensilDetector::GetMousePosition()
{
	return mousePosition;
}


